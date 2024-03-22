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

		//Constant Buffer Data를 생성.
		_cbFirstBase = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>>();
		_cbAltogetherSkinned = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinned>>();

		//Bone의 수만큼 GPU에 들어갈 벡터의 크기를 설정해야 한다. (ASSET_MAXIMUM_BONE_NUMBER_PER_MESH)
		_boneTransformVector.resize(_modelData->_assetSkinnedData->_renderBoneInfoVector.size());

		for (int i = 0; i < Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH; i++)
		{
			_cbAltogetherSkinned->GetDataStruct()->gCBuf_Bones[i] = DirectX::SimpleMath::Matrix::Identity;
		}
	}

	RenderObjectSkinnedMesh3D::~RenderObjectSkinnedMesh3D()
	{

	}

	void RenderObjectSkinnedMesh3D::CreateObjMatBuffers()
	{
		//VB 로드. *(Index Buffer는 공유)
		GraphicsResourceManager::Instance()->GetBasic3DLoader()->LoadObjMatBufferSkinned(_objMatVB, _modelData, _objectID, _materialID);

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
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stSkinned);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);

		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectSkinnedMesh3D::First_Render(const float* const dt)
	{

		BindMainVertexIndexBuffer();


	}

	void RenderObjectSkinnedMesh3D::First_UnbindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		UpdateObjMatBaseCB(camData);
		UpdateObjMatSkinnedCB();
	}

	void RenderObjectSkinnedMesh3D::ObjMat_BindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_Render(const float* const dt)
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_UnbindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::SetAnimation(const std::string& animName, bool isLoop)
	{
		assert(_modelData->_assetSkinnedData->_viableAnimations.contains(animName) &&
			"걸리면 유효하지 않은 Animation 로드 시도한 것");

		this->_currentAnim = _modelData->_assetSkinnedData->_viableAnimations.at(animName);

		RefreshStartEndAnim();

		//isLoop따라 값 설정. 해당 값은 일반적으로 데스등 장면에 활용될 것이니, 명시적으로 TPOSE를 넣지는 않을 것이다.
		this->_isLoop = isLoop;
	}

	void RenderObjectSkinnedMesh3D::UpdateAnimationInfo(const float* const dt)
	{
		//실제로 값 업데이트.
		UpdateAnimMatrices(*dt);

		//Script 딴에서 로직 처리가 되었을 것이다.
		const auto tNowTime = std::chrono::steady_clock::now();
		const auto tPassedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(tNowTime - _startedTime);

		if (tNowTime <= _expectedEndTime)
		{
			//아직까지는 예전에 정의된 애니메이션이 마무리되지 않은 것이다.
			float tPassedDurSec = static_cast<float>(tPassedDuration.count()) / 1000.0f;
			double tUnmanagedTick = (_currentAnim->_animAssetData->_ticksPerSecond) * tPassedDurSec;
			//Time 연산 실패로 값이 빠져나가는 것을 막기 위해서, CLAMP. 현재 Tick 값 투입.
			this->_currentTick = std::clamp<double>(tUnmanagedTick, 0, _currentAnim->_animAssetData->_durationTick);
		}
		else
		{
			//마무리 예정 시간보다 더 오랜 시간이 지났다. 판단을 내릴 때.
			if (_isLoop)
			{
				RefreshStartEndAnim();
			}
			else
			{
				//마지막 프레임에 머물러 있게 해야 한다.
				this->_currentTick = _currentAnim->_animAssetData->_durationTick;
			}
		}

	}

	void RenderObjectSkinnedMesh3D::UpdateAnimMatrices(float dt)
	{
		//더 이상 Recursive하게 들어갈 필요가 없다.
		//Linear하게 되었기 때문.

		//0번째 인덱스 == RootNode. pair의 second가 비었다.
		//부모가 없다는 얘기, RootNode이어서이다. 딱 한번 발생. -> SKIP.

		//현재 Tick, Animation, Parent Matrix.
		//둘 다 인덱스 동일하게 적용.
		const auto& tHierNodes = _modelData->_assetSkinnedData->_linearizedNodeHierarchy;
		const auto& tNodeAnims = _currentAnim->_animAssetData->_linearizedNodeAnimList;

		//모든 Node들 검사.
		for (int i = 0; i < tHierNodes.size(); i++)
		{
			if (tNodeAnims.at(i) != nullptr)
			{
				UpdateSingleNodeWithAnim(tHierNodes[i].first, tHierNodes[i].second, tNodeAnims.at(i));
			}
		}
	}

	void RenderObjectSkinnedMesh3D::RefreshStartEndAnim()
	{
		//다시 시간을 설정한 뒤에 시작한다.
		this->_startedTime = std::chrono::steady_clock::now();

		double tSecondsPerTick = 1.0f / (_currentAnim->_animAssetData->_durationTick);
		float tDurationTickSec = static_cast<float>(tSecondsPerTick * (_currentAnim->_animAssetData->_durationTick));
		//Ex. 3.5초면 3500 millisecond가 되어야 한다.
		int tMS_TS = tDurationTickSec * 1000;

		this->_expectedEndTime = _startedTime + std::chrono::milliseconds(tMS_TS);

		//현재 Tick Reset.
		this->_currentTick = 0;
	}

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
		_cbFirstBase->GetDataStruct()->gCBuf_WorldView = tViewTMMat;
		_cbFirstBase->GetDataStruct()->gCBuf_WorldViewProj = DirectX::XMMatrixMultiply(tWorldTMMat, DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
		_cbFirstBase->GetDataStruct()->gCBuf_CameraPositionW = tCameraPositionW;

		//첫번째 Constant Buffer에는 얘만 넣어주면 된다.
		_cbFirstBase->Update();
	}

	void RenderObjectSkinnedMesh3D::UpdateSkinnedCB()
	{
		//이미 UpdateAnimation으로 인해 업데이트가 되어 있는 상태.
		assert(_boneTransformVector.size() < 100 && "100 이내, 하드웨어의 한도!");

		for (int i = 0; i < _boneTransformVector.size(); i++)
		{
			_cbAltogetherSkinned->GetDataStruct()->gCBuf_Bones[i] = _boneTransformVector.at(i);
		}

		//값이 맞게 들어갔으니, 업데이트.
		_cbAltogetherSkinned->Update();
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
		//값이 이미 맞게 들어가 있으니, 업데이트.
		_cbAltogetherSkinned->Update();
	}

	void RenderObjectSkinnedMesh3D::CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		using namespace DirectX::SimpleMath;
		using namespace DirectX;

		//키가 한개 밖에 없으면 보간할 수 없다.
		if (pNodeAnim->_numRotationKeys == 1)
		{
			outQuat = pNodeAnim->_rotationKeyList[0]._value;
			return;
		}

		// 현재 Rotation 키프레임 획득.
		unsigned int RotationIndex = FindRotationIndex(animTick, pNodeAnim);

		// 다음 Rotation Key Index 범위 내부인지 체크.
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->_numRotationKeys);

		//키프레임 사이에서의 시간 차이를 구한다.
		float DeltaTime = pNodeAnim->_rotationKeyList[NextRotationIndex]._time - pNodeAnim->_rotationKeyList[RotationIndex]._time;

		// 시간 변화 내부 현재 지난 시간을 구한다 -> 보간 비율.  
		float Factor = (animTick - (float)pNodeAnim->_rotationKeyList[RotationIndex]._time) / DeltaTime;

		///오류 : Factor가 0~1 사이로 결정되지 않는 오류 확인!
		// 현재-다음 키프레임들의 회전을 위한 쿼터니언 둘 다 구한다.
		const Quaternion StartRotationQ = pNodeAnim->_rotationKeyList[RotationIndex]._value;
		const Quaternion EndRotationQ = pNodeAnim->_rotationKeyList[NextRotationIndex]._value;

		// Interpolate between them using the Factor. 
		//Quaternion::Slerp(StartRotationQ, EndRotationQ, Factor, outQuat);

		//더 안전한 Slerp를 사용.
		//TRY:
		outQuat = MathHelper::QuaternionSlerpNoFlip(StartRotationQ, EndRotationQ, Factor);

		// 정규화 후 참조자로 리턴.
		outQuat.Normalize();
	}

	void RenderObjectSkinnedMesh3D::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		//키가 한개 밖에 없으면 보간할 수 없다.
		if (pNodeAnim->_numPositionKeys == 1)
		{
			outVec = pNodeAnim->_positionKeyList[0]._value;
			return;
		}

		//적합한 Position 인덱스인지 확인.
		unsigned int PositionIndex = FindTranslationIndex(animTick, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->_numPositionKeys);

		//키프레임 사이에서의 시간 차이를 구한다.
		float DeltaTime = pNodeAnim->_positionKeyList[NextPositionIndex]._time - pNodeAnim->_positionKeyList[PositionIndex]._time;
		
		// 시간 변화 내부 현재 지난 시간을 구한다 -> 보간 비율.  
		float Factor = (animTick - (float)pNodeAnim->_positionKeyList[PositionIndex]._time) / DeltaTime;

		//전/후 보간할 Vector3 값 구하기.
		const DirectX::SimpleMath::Vector3 Start = pNodeAnim->_positionKeyList[PositionIndex]._value;
		const DirectX::SimpleMath::Vector3 End = pNodeAnim->_positionKeyList[NextPositionIndex]._value;

		DirectX::SimpleMath::Vector3 Delta = End - Start;

		//참조자로 리턴.
		outVec = Start + Factor * Delta;
	}

	unsigned int RenderObjectSkinnedMesh3D::FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		// Rotation 키 프레임들이 있는지 확인.
		assert(pNodeAnim->_numRotationKeys > 0);

		//현재 애니메이션 시간 직전의 Rotation을 찾고 인덱스 리턴.
		for (unsigned int i = 0; i < pNodeAnim->_numRotationKeys - 1; i++)
		{
			//FIX.
			if (animTick < (float)pNodeAnim->_rotationKeyList[i + 1]._time)
			{
				return i;
			}
		}

		//여기까지 오면 못찾은 것 == 실패한 것.
		assert(false);

		return 0;
	}

	unsigned int RenderObjectSkinnedMesh3D::FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		assert(pNodeAnim->_numPositionKeys > 0);

		// Find the translation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->_numPositionKeys - 1; i++)
		{
			//FIX.
			if (animTick < (float)pNodeAnim->_positionKeyList[i + 1]._time)
			{
				return i;
			}
		}

		//여기까지 오면 못찾은 것 == 실패한 것.
		assert(false);

		return 0;
	}

	void RenderObjectSkinnedMesh3D::UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim)
	{
		using namespace DirectX::SimpleMath;
		using namespace DirectX;

		Matrix tNodeTransformation = selfNode->_relTransform;

		//일단 남겨두기. Single-Mesh Animation이기에 가능. 
		Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f));
		tNodeTransformation = rotationMatrix;

		// Rotation 보간.
		Quaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, _currentTick, nodeAnim);
		Matrix RotationM = DirectX::XMMatrixRotationQuaternion(RotationQ);

		//Translation 보간.
		DirectX::SimpleMath::Vector3 Translation;
		CalcInterpolatedTranslation(Translation, _currentTick, nodeAnim);
		Matrix TranslationM;
		TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

		//Rotation, Translation 결합.
		tNodeTransformation = RotationM * TranslationM;

		//RootNode <-> 일반 Node 구별을 위해.
		Matrix tParentTransform = Matrix::Identity;
		if (parentNode != nullptr)
		{
			tParentTransform = parentNode->_relTransform;
		}

		//Global Transformation 찾기.
		Matrix tGlobalTransformation = tNodeTransformation * tParentTransform;

		const auto& tMappedBones = _modelData->_assetSkinnedData->_mappedBones;
		const auto& tBoneInfoVec = _modelData->_assetSkinnedData->_renderBoneInfoVector;
		const auto& tMeshGlobalInvTRS = _modelData->_assetSkinnedData->_meshGlobalInverseTransform;

		//최적화적으로 과히 좋지는 않지만, MappedBones 찾기.
		if (tMappedBones.find(selfNode->_nodeName) != tMappedBones.end())
		{
			UINT tBoneIndex = tMappedBones.at(selfNode->_nodeName);

			// = Final Transform의 역할까지 수행.
			_boneTransformVector[tBoneIndex] =
				tBoneInfoVec[tBoneIndex]._boneOffset * tGlobalTransformation * tMeshGlobalInvTRS;
		}
	}



}