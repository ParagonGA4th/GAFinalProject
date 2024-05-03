#include "RenderObjectSkinnedMesh3D.h"

#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "MathHelper.h"
#include "RenderAnimation.h"
#include "RenderTexture2D.h"
#include "AssetBasic3DLoader.h"
#include "AssetModelDataDefine.h"
#include "AssetTextureType.h"
#include "MaterialCluster.h"

#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/AnimTransform.h"

#include <dxtk/WICTextureLoader.h>
#include <dxtk/SimpleMath.h>
#include <singleton-cpp/singleton.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectSkinnedMesh3D::RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID) : RenderObject3D(baseRenderer, objID)
	{
		//SkinnedMesh도 똑같은 과정 거쳐야.
		//StaticMeshRenderer 따로 포인터를 받기.
		//Mesh 데이터를 받기.
		//VB/IB 로드.
		//Constant Buffer Data를 생성.

		//StaticMeshRenderer 따로 포인터를 받기.
		Pg::Data::SkinnedMeshRenderer* tSkinnedMeshRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(GetBaseRenderer());

		//Mesh 데이터를 받기.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tSkinnedMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());

		//개별적으로 렌더에 쓰일 CopyModifiableNode : 생성.
		_copiedModifyRootNode = std::make_unique<ModifiedNode_SkinnedMesh>(nullptr);

		_copiedModifyRootNode->_relTransform->SetParent(nullptr, false);
		_copiedModifyRootNode->RecursiveInitFromNode(_modelData->_assetSceneData->_rootNode.get(), _animatedModifNodeMap);

		//Constant Buffer Data를 생성.
		_cbFirstBase = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>>();
		_cbAllSkinnedNodes = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedNodes>>();
		_cbAllSkinnedBones = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedBones>>();

		for (int i = 0; i < Pg::Defines::ASSET_MAXIMUM_NODE_NUMBER_PER_MESH; i++)
		{
			_cbAllSkinnedNodes->GetDataStruct()->gCBuf_Nodes[i] = DirectX::SimpleMath::Matrix::Identity;
		}

		for (int i = 0; i < Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH; i++)
		{
			_cbAllSkinnedBones->GetDataStruct()->gCBuf_Bones[i] = DirectX::SimpleMath::Matrix::Identity;
		}
	}

	RenderObjectSkinnedMesh3D::~RenderObjectSkinnedMesh3D()
	{

	}

	void RenderObjectSkinnedMesh3D::CreateObjMatBuffers()
	{
		//VB 로드. *(Index Buffer는 공유)
		GraphicsResourceManager::Instance()->GetBasic3DLoader()->LoadObjMatBufferSkinned(_3rdVB, _modelData, _objectID, GetMaterialID());

		//Constant Buffer Data를 생성. Skinned는 재사용. 
		_cbObjMatBase = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>>();
	}

	void RenderObjectSkinnedMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		UpdateMainCB(camData);
		UpdateSkinnedCB();
	}

	void RenderObjectSkinnedMesh3D::First_BindBuffers()
	{
		_cbFirstBase->BindVS(0);
		_cbAllSkinnedNodes->BindVS(1);
		_cbAllSkinnedBones->BindVS(2);
	}

	void RenderObjectSkinnedMesh3D::First_Render(const float* const dt)
	{
		BindMainVertexIndexBuffer();

		//아트에서 들어오는 리소스들은 모두 싱글Mesh이지만,
		//FBX에 들어오면서부터는 내부의 다른 Material 사용으로 쪼개질 것이다.
		//애니메이션 연산에는 연관X, 형식 유지를 위해 출력.

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;
		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectSkinnedMesh3D::First_UnbindBuffers()
	{
		_cbFirstBase->UnbindVS(0);
		_cbAllSkinnedNodes->UnbindVS(1);
		_cbAllSkinnedBones->UnbindVS(2);
	}

	void RenderObjectSkinnedMesh3D::ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		UpdateObjMatBaseCB(camData);
		UpdateObjMatSkinnedCB();
	}

	void RenderObjectSkinnedMesh3D::ObjMat_BindBuffers()
	{
		_cbObjMatBase->BindVS(0);
		_cbAllSkinnedNodes->BindVS(1);
		_cbAllSkinnedBones->BindVS(2);

		// PixelShader : 이제 Albedo / Normal / Specular / Arm 데이터를 넣어줘야 한다.
		// 디폴트 매터리얼 상관하지 않고, 모든 오브젝트가 값 자체는 이제 필요하게 될 것이라는 말이다. Texture 투입.
		// 그냥 예전방식대로, Texture2DArray 자체를 투입할 것.
		// 나중에는 같은 오브젝트 + 인스턴싱의 영향을 받는다면 해당 스텝을 누락하던가, 

		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(_modelData->_pbrTextureArrays[0]->GetSRV()));
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(_modelData->_pbrTextureArrays[1]->GetSRV()));
		// Specular
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(_modelData->_pbrTextureArrays[2]->GetSRV()));
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(_modelData->_pbrTextureArrays[3]->GetSRV()));
	}

	void RenderObjectSkinnedMesh3D::ObjMat_Render(const float* const dt)
	{
		BindObjMatVertexIndexBuffer();

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectSkinnedMesh3D::ObjMat_UnbindBuffers()
	{
		_cbObjMatBase->UnbindVS(0);
		_cbAllSkinnedNodes->UnbindVS(1);
		_cbAllSkinnedBones->UnbindVS(2);

		//PBR Texture를 다 썼으니, 이제 할당 해제!
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		// PBR Texture Arrays To NULL
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(tNullSRV));
	}

	void RenderObjectSkinnedMesh3D::SetAnimation(const std::string& animName, bool isLoop)
	{
		assert(_modelData->_assetSkinnedData->_viableAnimations.contains(animName) &&
			"걸리면 유효하지 않은 Animation 로드 시도한 것");

		this->_currentAnim = _modelData->_assetSkinnedData->_viableAnimations.at(animName);

		//명시적으로 돌리는 시간 리셋.
		_animationTime = 0.0;
		_currentTick = 0.0;

		//isLoop따라 값 설정. 해당 값은 일반적으로 데스등 장면에 활용될 것이니, 명시적으로 TPOSE를 넣지는 않을 것이다.
		this->_isLoop = isLoop;
	}

	void RenderObjectSkinnedMesh3D::UpdateAnimationInfo(const float* const dt, const Pg::Data::Enums::eEditorMode* const editorMode)
	{
		if ((*editorMode) == Data::Enums::eEditorMode::_NONE ||
			(*editorMode) == Data::Enums::eEditorMode::_EDIT)
		{
			_animationTime = 0.0;
			_currentTick = 0.0;

			//Animation을 돌리지 않는다.
			UpdateAnimMatrices(0.0f);
		}
		else
		{
			//Script 딴에서 로직 처리가 되었을 것이다.
			_animationTime += (*dt);
			_currentTick = _animationTime * _currentAnim->_animAssetData->_ticksPerSecond;

			if (_currentTick > _currentAnim->_animAssetData->_durationTick)
			{
				if (_isLoop)
				{
					double secondPerTick = _currentAnim->_animAssetData->_durationTick / _currentAnim->_animAssetData->_ticksPerSecond;
					int count = 0;
					while (secondPerTick * (count + 1) < _animationTime)
					{
						count++;
					}
					_animationTime -= count * secondPerTick;
					_currentTick = _animationTime * _currentAnim->_animAssetData->_ticksPerSecond;
				}
				else
				{
					_animationTime = _currentAnim->_animAssetData->_durationTick / _currentAnim->_animAssetData->_ticksPerSecond;
					_currentTick = _animationTime * _currentAnim->_animAssetData->_ticksPerSecond;
				}
			}

			//실제로 값 업데이트.
			UpdateAnimMatrices(*dt);

		}

		//실제로 Animation대로 연산된 값을 알맞은 행렬의 배열로 투입.
		FillInNodeBuffer(_copiedModifyRootNode.get());
		FillInBoneBuffer(_copiedModifyRootNode.get());
		assert("");
	}

	void RenderObjectSkinnedMesh3D::UpdateAnimMatrices(float dt)
	{
		/////TOREMOVE
		//this->_currentTick = 0;


		for (auto& nodeAnim : _currentAnim->_animAssetData->_channelList)
		{
			DirectX::SimpleMath::Vector3 position;
			DirectX::SimpleMath::Vector4 rotation;

			const ModifiedNode_SkinnedMesh* node = _animatedModifNodeMap[nodeAnim->_nodeName];
			//무조건 NodeAnim은 Node와 매칭되어야 하는데..?
			if (node == nullptr)
			{
				//애초에 못 찾았으면 안되는데.. 원래 FBX에 없었던 값이 채워지는 것 같다.
				//Armature.002라는 프로퍼티가 문제됨.
				//일단은 무시할 것.
				continue;
			}

			//TODO : NodeAnim 없는 경우 대비.
			
			// Position
			{
				int positionIndex = 0;
				for (int i = 0; i < nodeAnim->_numPositionKeys; i++)
				{
					if (_currentTick < nodeAnim->_positionKeyList[i]._time)
					{
						positionIndex = i;		// i-1 < _animationTickTime < i
						break;
					}
				}

				if (positionIndex == 0)
				{
					position = nodeAnim->_positionKeyList[0]._value;
				}

				else
				{
					double t = (_currentTick - nodeAnim->_positionKeyList[positionIndex - 1]._time) / (nodeAnim->_positionKeyList[positionIndex]._time - nodeAnim->_positionKeyList[positionIndex - 1]._time);
					position = DirectX::XMVectorLerp(nodeAnim->_positionKeyList[positionIndex - 1]._value, nodeAnim->_positionKeyList[positionIndex]._value, (float)t);
				}
			}

			// Rotation
			{
				int rotationIndex = 0;
				for (int i = 0; i < nodeAnim->_numRotationKeys; i++)
				{
					if (_currentTick < nodeAnim->_rotationKeyList[i]._time)
					{
						rotationIndex = i;
						break;
					}
				}

				if (rotationIndex == 0)
				{
					rotation = nodeAnim->_rotationKeyList[0]._value;
				}
				else
				{
					double t = (_currentTick - nodeAnim->_rotationKeyList[rotationIndex - 1]._time) / (nodeAnim->_rotationKeyList[rotationIndex]._time - nodeAnim->_rotationKeyList[rotationIndex - 1]._time);
					rotation = DirectX::XMQuaternionSlerp(nodeAnim->_rotationKeyList[rotationIndex - 1]._value, nodeAnim->_rotationKeyList[rotationIndex]._value, (float)t);
				}
			}

			auto tMat = DirectX::XMMatrixAffineTransformation({1,1,1}, { 0,0,0,0 }, rotation, position);

			//node->_relTransform->_position = { position.x, position.y, position.z};
			//node->_relTransform->_rotation = { rotation.w, rotation.x, rotation.y, rotation.z };

			node->_relTransform->SetLocalPosition(position);
			node->_relTransform->SetLocalRotation(rotation);
		
			//Scale은 서포트하지 않는다. 다만, 0.01을 반영..?
			//node->_relTransform->SetLocalScale({ 1.0f,1.0f, 1.0f });
			//node->_relTransform->SetLocalScale({ 0.01f, 0.01f, 0.01f });
		}
	}

	//void RenderObjectSkinnedMesh3D::RefreshStartEndAnim()
	//{
	//	//다시 시간을 설정한 뒤에 시작한다.
	//	this->_startedTime = std::chrono::steady_clock::now();
	//
	//	double tSecondsPerTick = 1.0f / (_currentAnim->_animAssetData->_durationTick);
	//	float tDurationTickSec = static_cast<float>(tSecondsPerTick * (_currentAnim->_animAssetData->_durationTick));
	//	//Ex. 3.5초면 3500 millisecond가 되어야 한다.
	//	int tMS_TS = tDurationTickSec * 1000;
	//
	//	this->_expectedEndTime = _startedTime + std::chrono::milliseconds(tMS_TS);
	//
	//	//현재 Tick Reset.
	//	this->_currentTick = 0;
	//}

	void RenderObjectSkinnedMesh3D::BindMainVertexIndexBuffer()
	{
		//Vertex Buffer Skinned.
		UINT stride = sizeof(LayoutDefine::Vin1stSkinned);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectSkinnedMesh3D::UpdateMainCB(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		// 상수버퍼에 들어갈 값 셋팅
		DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldTMMat));

		//0.01 스케일링 적용.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMFLOAT3 tCameraPositionW = Helper::MathHelper::PG2XM_FLOAT3(camData->_position);
		DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPositionW);
		DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);

		float tCamDistance = 0.0f;
		DirectX::XMStoreFloat(&tCamDistance, DirectX::XMVector3Length(tCameraPositionVec));

		_cbFirstBase->GetDataStruct()->gCBuf_World = tWorldTMMat;
		_cbFirstBase->GetDataStruct()->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;
		_cbFirstBase->GetDataStruct()->gCBuf_WorldView = DirectX::XMMatrixMultiply(tWorldTMMat, tViewTMMat);
		_cbFirstBase->GetDataStruct()->gCBuf_WorldViewProj = DirectX::XMMatrixMultiply(tWorldTMMat, DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
		_cbFirstBase->GetDataStruct()->gCBuf_CameraPositionW = tCameraPositionW;

		//첫번째 Constant Buffer에는 얘만 넣어주면 된다.
		_cbFirstBase->Update();
	}

	void RenderObjectSkinnedMesh3D::UpdateSkinnedCB()
	{
		// 따로 이제 값을 설정하는 것이 아닌, 업데이트 로직에 의해 이미 값이 들어가 있는 상태이다.
		_cbAllSkinnedNodes->Update();
		_cbAllSkinnedBones->Update();
	}

	void RenderObjectSkinnedMesh3D::UpdateObjMatBaseCB(Pg::Data::CameraData* camData)
	{
		DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		//0.01 스케일링 적용.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		_cbObjMatBase->GetDataStruct()->gCBuf_World = tWorldTMMat;
		_cbObjMatBase->GetDataStruct()->gCBuf_WorldViewProj = DirectX::XMMatrixMultiply(tWorldTMMat, DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		//값이 맞게 들어갔으니, 업데이트.
		_cbObjMatBase->Update();
	}

	void RenderObjectSkinnedMesh3D::UpdateObjMatSkinnedCB()
	{
		// 이미 FirstBase 단계에서 값이 맞게 들어갔으니, 업데이트.
		_cbAllSkinnedNodes->Update();
		_cbAllSkinnedBones->Update();
	}


	void RenderObjectSkinnedMesh3D::BindObjMatVertexIndexBuffer()
	{
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::VinPerObjMatIDSkinned);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_3rdVB), &stride, &offset);
		//Index Buffer Setting. (Model Data와 공유)
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	}

	void RenderObjectSkinnedMesh3D::FillInNodeBuffer(const ModifiedNode_SkinnedMesh* const selfNode)
	{
		// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
		// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
		// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

		if (selfNode->_index >= 0)
		{
			// nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
			
			DirectX::SimpleMath::Matrix tInputData = DirectX::XMMatrixTranspose(selfNode->_relTransform->GetWorldTM());
			DirectX::SimpleMath::Matrix tZeroMat = DirectX::XMMatrixSet(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
			assert(tInputData != tZeroMat);

			//일단 외적으로 Decompose 및 재투입은 하지 않은 상황.
			_cbAllSkinnedNodes->GetDataStruct()->gCBuf_Nodes[selfNode->_index] = tInputData;
				

			//_cbAllSkinnedNodes->GetDataStruct()->gCBuf_Nodes[selfNode->_index] = selfNode->_relTransform->GetWorldTM();
		}

		//Early Return
		if (selfNode->_numChildren == 0)
		{
			return;
		}

		//nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->worldTM);
		for (int i = 0; i < selfNode->_childrenList.size(); i++)
		{
			FillInNodeBuffer(selfNode->_childrenList[i].get());
		}
	}

	void RenderObjectSkinnedMesh3D::FillInBoneBuffer(const ModifiedNode_SkinnedMesh* const selfNode)
	{
		BoneInfo_AssetData* bone = selfNode->_bindedBone;

		if (bone)
		{
			DirectX::SimpleMath::Matrix tInputData = DirectX::XMMatrixTranspose(bone->_offsetMatrix);
			DirectX::SimpleMath::Matrix tZeroMat = DirectX::XMMatrixSet(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

			assert(tInputData != tZeroMat);
			_cbAllSkinnedBones->GetDataStruct()->gCBuf_Bones[bone->_index] = tInputData;
			//_cbAllSkinnedBones->GetDataStruct()->gCBuf_Bones[bone->_index] = bone->_offsetMatrix;
		}

		//Early Return
		if (selfNode->_numChildren == 0)
		{
			return;
		}

		for (int i = 0; i < selfNode->_childrenList.size(); i++)
		{
			FillInBoneBuffer(selfNode->_childrenList[i].get());
		}
	}

	Pg::Data::AnimTransform* RenderObjectSkinnedMesh3D::FindAnimTransform(const std::string& animNodeName)
	{
		assert(_animatedModifNodeMap.contains(animNodeName) && "본 이름이 기록되어 있지 않다!");

		const ModifiedNode_SkinnedMesh* tVal = _animatedModifNodeMap.at(animNodeName);
		return tVal->_relTransform.get();
	}

}

#pragma region OldSkinningCode
//void RenderObjectSkinnedMesh3D::CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	using namespace DirectX::SimpleMath;
	//	using namespace DirectX;
	//
	//	//키가 한개 밖에 없으면 보간할 수 없다.
	//	if (pNodeAnim->_numRotationKeys == 1)
	//	{
	//		outQuat = pNodeAnim->_rotationKeyList[0]._value;
	//		return;
	//	}
	//
	//	// 현재 Rotation 키프레임 획득.
	//	unsigned int RotationIndex = FindRotationIndex(animTick, pNodeAnim);
	//
	//	// 다음 Rotation Key Index 범위 내부인지 체크.
	//	unsigned int NextRotationIndex = (RotationIndex + 1);
	//	assert(NextRotationIndex < pNodeAnim->_numRotationKeys);
	//
	//	//키프레임 사이에서의 시간 차이를 구한다.
	//	float DeltaTime = pNodeAnim->_rotationKeyList[NextRotationIndex]._time - pNodeAnim->_rotationKeyList[RotationIndex]._time;
	//
	//	// 시간 변화 내부 현재 지난 시간을 구한다 -> 보간 비율.  
	//	float Factor = (animTick - (float)pNodeAnim->_rotationKeyList[RotationIndex]._time) / DeltaTime;
	//
	//	///오류 : Factor가 0~1 사이로 결정되지 않는 오류 확인!
	//	// 현재-다음 키프레임들의 회전을 위한 쿼터니언 둘 다 구한다.
	//	const Quaternion StartRotationQ = pNodeAnim->_rotationKeyList[RotationIndex]._value;
	//	const Quaternion EndRotationQ = pNodeAnim->_rotationKeyList[NextRotationIndex]._value;
	//
	//	// Interpolate between them using the Factor. 
	//	//Quaternion::Slerp(StartRotationQ, EndRotationQ, Factor, outQuat);
	//
	//	//더 안전한 Slerp를 사용.
	//	//TRY:
	//	outQuat = MathHelper::QuaternionSlerpNoFlip(StartRotationQ, EndRotationQ, Factor);
	//
	//	// 정규화 후 참조자로 리턴.
	//	outQuat.Normalize();
	//}
	//
	//void RenderObjectSkinnedMesh3D::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	//키가 한개 밖에 없으면 보간할 수 없다.
	//	if (pNodeAnim->_numPositionKeys == 1)
	//	{
	//		outVec = pNodeAnim->_positionKeyList[0]._value;
	//		return;
	//	}
	//
	//	//적합한 Position 인덱스인지 확인.
	//	unsigned int PositionIndex = FindTranslationIndex(animTick, pNodeAnim);
	//	unsigned int NextPositionIndex = (PositionIndex + 1);
	//	assert(NextPositionIndex < pNodeAnim->_numPositionKeys);
	//
	//	//키프레임 사이에서의 시간 차이를 구한다.
	//	float DeltaTime = pNodeAnim->_positionKeyList[NextPositionIndex]._time - pNodeAnim->_positionKeyList[PositionIndex]._time;
	//
	//	// 시간 변화 내부 현재 지난 시간을 구한다 -> 보간 비율.  
	//	float Factor = (animTick - (float)pNodeAnim->_positionKeyList[PositionIndex]._time) / DeltaTime;
	//
	//	//전/후 보간할 Vector3 값 구하기.
	//	const DirectX::SimpleMath::Vector3 Start = pNodeAnim->_positionKeyList[PositionIndex]._value;
	//	const DirectX::SimpleMath::Vector3 End = pNodeAnim->_positionKeyList[NextPositionIndex]._value;
	//
	//	DirectX::SimpleMath::Vector3 Delta = End - Start;
	//
	//	//참조자로 리턴.
	//	outVec = Start + Factor * Delta;
	//}
	//
	//unsigned int RenderObjectSkinnedMesh3D::FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	// Rotation 키 프레임들이 있는지 확인.
	//	assert(pNodeAnim->_numRotationKeys > 0);
	//
	//	//현재 애니메이션 시간 직전의 Rotation을 찾고 인덱스 리턴.
	//	for (unsigned int i = 0; i < pNodeAnim->_numRotationKeys - 1; i++)
	//	{
	//		//FIX.
	//		if (animTick < (float)pNodeAnim->_rotationKeyList[i + 1]._time)
	//		{
	//			return i;
	//		}
	//	}
	//
	//	//여기까지 오면 못찾은 것 == 실패한 것.
	//	assert(false);
	//
	//	return 0;
	//}
	//
	//unsigned int RenderObjectSkinnedMesh3D::FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	assert(pNodeAnim->_numPositionKeys > 0);
	//
	//	// Find the translation key just before the current animation time and return the index. 
	//	for (unsigned int i = 0; i < pNodeAnim->_numPositionKeys - 1; i++)
	//	{
	//		//FIX.
	//		if (animTick < (float)pNodeAnim->_positionKeyList[i + 1]._time)
	//		{
	//			return i;
	//		}
	//	}
	//
	//	//여기까지 오면 못찾은 것 == 실패한 것.
	//	assert(false);
	//
	//	return 0;
	//}

	//void RenderObjectSkinnedMesh3D::UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim)
	//{
	//	using namespace DirectX::SimpleMath;
	//	using namespace DirectX;

	//	Matrix tNodeTransformation = selfNode->_relTransform;

	//	//일단 남겨두기. Single-Mesh Animation이기에 가능. 
	//	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f));
	//	tNodeTransformation = rotationMatrix;

	//	// Rotation 보간.
	//	Quaternion RotationQ;
	//	CalcInterpolatedRotation(RotationQ, _currentTick, nodeAnim);
	//	Matrix RotationM = DirectX::XMMatrixRotationQuaternion(RotationQ);

	//	//Translation 보간.
	//	DirectX::SimpleMath::Vector3 Translation;
	//	CalcInterpolatedTranslation(Translation, _currentTick, nodeAnim);
	//	Matrix TranslationM;
	//	TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

	//	//Rotation, Translation 결합.
	//	tNodeTransformation = RotationM * TranslationM;

	//	//RootNode <-> 일반 Node 구별을 위해.
	//	Matrix tParentTransform = Matrix::Identity;
	//	if (parentNode != nullptr)
	//	{
	//		tParentTransform = parentNode->_relTransform;
	//	}

	//	//Global Transformation 찾기.
	//	Matrix tGlobalTransformation = tNodeTransformation * tParentTransform;

	//	const auto& tMappedBones = _modelData->_assetSkinnedData->_mappedBones;
	//	const auto& tBoneInfoVec = _modelData->_assetSkinnedData->_renderBoneInfoVector;
	//	const auto& tMeshGlobalInvTRS = _modelData->_assetSkinnedData->_meshGlobalInverseTransform;

	//	//최적화적으로 과히 좋지는 않지만, MappedBones 찾기.
	//	if (tMappedBones.find(selfNode->_nodeName) != tMappedBones.end())
	//	{
	//		UINT tBoneIndex = tMappedBones.at(selfNode->_nodeName);

	//		// = Final Transform의 역할까지 수행.
	//		_boneTransformVector[tBoneIndex] =
	//			tBoneInfoVec[tBoneIndex]._boneOffset * tGlobalTransformation * tMeshGlobalInvTRS;
	//	}
	//}
#pragma endregion OldSkinningCode
