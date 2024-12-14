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
		//SkinnedMesh�� �Ȱ��� ���� ���ľ�.
		//StaticMeshRenderer ���� �����͸� �ޱ�.
		//Mesh �����͸� �ޱ�.
		//VB/IB �ε�.
		//Constant Buffer Data�� ����.

		//StaticMeshRenderer ���� �����͸� �ޱ�.
		Pg::Data::SkinnedMeshRenderer* tSkinnedMeshRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(GetBaseRenderer());

		//�ϴ�, ������ �޾ƿ���.
		ANIMATION_BLEND_SPEED_POINTER = &(tSkinnedMeshRenderer->_animBlendFactor);
		ANIMATION_PAUSE_PROPERTY_POINTER = &(tSkinnedMeshRenderer->_playVariable);

		//Mesh �����͸� �ޱ�.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tSkinnedMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());

		//���������� ������ ���� CopyModifiableNode : ����.
		_copiedModifyRootNode = std::make_unique<ModifiedNode_SkinnedMesh>(nullptr);

		_copiedModifyRootNode->_relTransform->SetParent(nullptr, false);
		_copiedModifyRootNode->RecursiveInitFromNode(_modelData->_assetSceneData->_rootNode.get(), _animatedModifNodeMap);

		//Constant Buffer Data�� ����.
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

		//Skinned ������ ����, Alpha �뵵.
		this->_skinnedMeshRenderer = tSkinnedMeshRenderer;
	}

	RenderObjectSkinnedMesh3D::~RenderObjectSkinnedMesh3D()
	{

	}

	void RenderObjectSkinnedMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		UpdateMainCB(camData);
		UpdateSkinnedCB();
	}

	void RenderObjectSkinnedMesh3D::First_BindBuffers()
	{
		_cbFirstBase->BindVS(0);
		_cbFirstBase->BindPS(0);
		_cbAllSkinnedNodes->BindVS(2);
		_cbAllSkinnedBones->BindVS(3);

		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(_modelData->_pbrTextureArrays[0]->GetSRV()));
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(_modelData->_pbrTextureArrays[1]->GetSRV()));
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(_modelData->_pbrTextureArrays[2]->GetSRV()));

		BindMainVertexIndexBuffer();
	}

	void RenderObjectSkinnedMesh3D::First_Render(const float* const dt)
	{
		//��Ʈ���� ������ ���ҽ����� ��� �̱�Mesh������,
		//FBX�� �����鼭���ʹ� ������ �ٸ� Material ������� �ɰ��� ���̴�.
		//�ִϸ��̼� ���꿡�� ����X, ���� ������ ���� ���.

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;
		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectSkinnedMesh3D::First_UnbindBuffers()
	{
		_cbFirstBase->UnbindVS(0);
		_cbFirstBase->UnbindPS(0);
		_cbAllSkinnedNodes->UnbindVS(2);
		_cbAllSkinnedBones->UnbindVS(3);

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);
	}

	void RenderObjectSkinnedMesh3D::SetAnimation(const std::string& animName, bool isLoop)
	{
		assert(_modelData->_assetSkinnedData->_viableAnimations.contains(animName) &&
			"�ɸ��� ��ȿ���� ���� Animation �ε� �õ��� ��");

		//PrevAnim ���. Nullptr�� �ƴϸ� ������ �����ؾ�!
		if ((*ANIMATION_BLEND_SPEED_POINTER) > std::numeric_limits<float>::epsilon())
		{
			//0�� �ƴϾ�� PrevAnim ��ü�� ���� �ִ´�.
			this->_prevAnim = this->_currentAnim;
		}
		this->_currentAnim = _modelData->_assetSkinnedData->_viableAnimations.at(animName);
		_blendLerpRatio = 0.0f;

		//��������� ������ �ð� ����.
		_animationTime = 0.0;
		_currentTick = 0.0;
		_isEndFrameCalled = false;

		//isLoop���� �� ����. �ش� ���� �Ϲ������� ������ ��鿡 Ȱ��� ���̴�, ��������� TPOSE�� ������ ���� ���̴�.
		this->_isLoop = isLoop;
	}

	void RenderObjectSkinnedMesh3D::UpdateAnimationInfo(const float* const dt, const Pg::Data::Enums::eEditorMode* const editorMode)
	{
		if ((*editorMode) == Data::Enums::eEditorMode::_NONE ||
			(*editorMode) == Data::Enums::eEditorMode::_EDIT)
		{
			_animationTime = 0.0;
			_currentTick = 0.0;

			//Animation�� ������ �ʴ´�.
			UpdateAnimMatrices(0.0f);
		}
		else
		{
			//Script ������ ���� ó���� �Ǿ��� ���̴�.
			//Play/Pause ����.
			float tPauseVar = (float)(*ANIMATION_PAUSE_PROPERTY_POINTER);

			_animationTime += (*dt) * tPauseVar;
			_currentTick = _animationTime * _currentAnim->_animAssetData->_ticksPerSecond;

			//������ ������ ���� �õ�.
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
					//_currentTick = _animationTime * _currentAnim->_animAssetData->_ticksPerSecond;
					_currentTick = _currentAnim->_animAssetData->_durationTick;

					if (!_isEndFrameCalled)
					{
						GetBaseRenderer()->_object->TurnOnAnimationEnd(_currentAnim->GetFileName());

						_isEndFrameCalled = true;
					}
				}
			}

			//������ �� ������Ʈ.
			UpdateAnimMatrices(*dt);

		}

		//������ Animation��� ����� ���� �˸��� ����� �迭�� ����.
		FillInNodeBuffer(_copiedModifyRootNode.get());
		FillInBoneBuffer(_copiedModifyRootNode.get());
		assert("");
	}

	void RenderObjectSkinnedMesh3D::UpdateAnimMatrices(float dt)
	{
		/////TOREMOVE
		//this->_currentTick = 0;

		//������ _prevAnim�� �����ؾ� �ϴ���, �ƴ��� �˻�.
		if (_prevAnim != nullptr)
		{
			////�ӵ� �� ����? �׷��� Lerp Ratio �� �̸�ŭ �� �߰��ؾ� ���� ��!
			//if ((*ANIMATION_BLEND_SPEED_POINTER) <= std::numeric_limits<float>::epsilon())
			//{
			//	//BlendSpeed�� 0���� �� ���, �ٷ� Switching �����ϰ� 1�� �ϵ��ڵ��ؼ� �Ѿ�� ���ش�.
			//	_blendLerpRatio = 100.f; // ������ ���ƿ���. 
			//}

			_blendLerpRatio += (dt * (*ANIMATION_BLEND_SPEED_POINTER));

			if (_blendLerpRatio >= 1.0f)
			{
				//�ϴ� ���� �ٽ� 0���� ������.
				_blendLerpRatio = 0.f;
				_prevAnim = nullptr;
			}
		}

		for (auto& nodeAnim : _currentAnim->_animAssetData->_channelList)
		{
			DirectX::SimpleMath::Vector3 position;
			DirectX::SimpleMath::Quaternion rotation;
			DirectX::SimpleMath::Vector3 scale;

			//NodeAnim�� �߿��� ���� : Name��, ���� ã�� ���ؼ�.
			const ModifiedNode_SkinnedMesh* node = _animatedModifNodeMap[nodeAnim->_nodeName];

			//������ NodeAnim�� Node�� ��Ī�Ǿ�� �ϴµ�..?
			if (node == nullptr)
			{
				//�ϴ��� �̷��� ó��.
				continue;
			}

			if (_prevAnim != nullptr)
			{
				auto& bPrevChannelList = _prevAnim->_animAssetData->_channelList;	//PrevAnim���� ������ NodeAnim�� ã�ƾ� �Ѵ�.
				NodeAnim_AssetData* tPrevFoundNodeAnim = nullptr;					//�ڽ��� �̸��� �̸��� ���� NodeAnim ã��.
				for (auto& bPrevNodeAnim : bPrevChannelList)
				{
					if (bPrevNodeAnim->_nodeName.compare(nodeAnim->_nodeName) == 0)
					{
						tPrevFoundNodeAnim = bPrevNodeAnim.get();
						break;
					}
				}

				if (tPrevFoundNodeAnim != nullptr)
				{
					//ã�Ҵ�.
					//Blending �ʿ�.
					DirectX::SimpleMath::Vector3 prev_position = FillPositionForNodeAnim(_prevAnim, tPrevFoundNodeAnim);
					DirectX::SimpleMath::Quaternion prev_rotation = FillRotationForNodeAnim(_prevAnim, tPrevFoundNodeAnim);
					//DirectX::SimpleMath::Vector3 prev_scale = FillScaleForNodeAnim(_prevAnim, tPrevFoundNodeAnim);

					DirectX::SimpleMath::Vector3 now_position = FillPositionForNodeAnim(_currentAnim, nodeAnim.get());
					DirectX::SimpleMath::Quaternion now_rotation = FillRotationForNodeAnim(_currentAnim, nodeAnim.get());
					//DirectX::SimpleMath::Vector3 now_scale = FillScaleForNodeAnim(_currentAnim, nodeAnim.get());

					assert(_blendLerpRatio <= 1.0f);
					position = DirectX::SimpleMath::Vector3::Lerp(prev_position, now_position, _blendLerpRatio);
					rotation = DirectX::SimpleMath::Quaternion::Slerp(prev_rotation, now_rotation, _blendLerpRatio);
					//scale = DirectX::SimpleMath::Vector3::Lerp(prev_scale, now_scale, _blendLerpRatio);
				}
				else
				{
					position = FillPositionForNodeAnim(_currentAnim, nodeAnim.get());
					rotation = FillRotationForNodeAnim(_currentAnim, nodeAnim.get());
					//scale = FillScaleForNodeAnim(_currentAnim, nodeAnim.get());
				}
			}
			else
			{
				//�׳� ����ϸ� ��.
				position = FillPositionForNodeAnim(_currentAnim, nodeAnim.get());
				rotation = FillRotationForNodeAnim(_currentAnim, nodeAnim.get());
				//scale = FillScaleForNodeAnim(_currentAnim, nodeAnim.get());
			}

			rotation.Normalize();
			node->_relTransform->SetLocalPosition(position);
			node->_relTransform->SetLocalRotation(rotation);
			//node->_relTransform->SetLocalScale(scale); // ������ �ݿ� ���Ѵ�.
		}
	}

	//void RenderObjectSkinnedMesh3D::RefreshStartEndAnim()
	//{
	//	//�ٽ� �ð��� ������ �ڿ� �����Ѵ�.
	//	this->_startedTime = std::chrono::steady_clock::now();
	//
	//	double tSecondsPerTick = 1.0f / (_currentAnim->_animAssetData->_durationTick);
	//	float tDurationTickSec = static_cast<float>(tSecondsPerTick * (_currentAnim->_animAssetData->_durationTick));
	//	//Ex. 3.5�ʸ� 3500 millisecond�� �Ǿ�� �Ѵ�.
	//	int tMS_TS = tDurationTickSec * 1000;
	//
	//	this->_expectedEndTime = _startedTime + std::chrono::milliseconds(tMS_TS);
	//
	//	//���� Tick Reset.
	//	this->_currentTick = 0;
	//}

	void RenderObjectSkinnedMesh3D::BindMainVertexIndexBuffer()
	{
		//Vertex Buffer Setting.
		UINT stride[2] = { sizeof(LayoutDefine::Vin1stSkinned_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual) };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* buffers[2] = { _modelData->_vertexBuffer, _modelData->_secondVertexBuffer };
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectSkinnedMesh3D::UpdateMainCB(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		//�ӽ÷� Translate���� �ٸ��� �����ϰ�, �ٽ� ���� �����Ѵ�.
		Pg::Math::PGFLOAT3 tOriginalPosValue = GetBaseRenderer()->_object->_transform._position;
		GetBaseRenderer()->_object->_transform._position = tOriginalPosValue + _rendererBase3DStorage->GetRendererOffset();

		// ������ۿ� �� �� ����
		//DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		//DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();
		if (_rendererBase3DStorage->GetIsUseRotationOriginOffset())
		{
			//����, Parent�� ����� �Ѵ�.
			auto& tTrans = GetBaseRenderer()->_object->_transform;
			DirectX::XMMATRIX tScaleMat = DirectX::XMMatrixScaling(tTrans._scale.x, tTrans._scale.y, tTrans._scale.z);
			DirectX::XMMATRIX tOrigRotMat = Pg::Math::PG2XM_MATRIX4X4(Pg::Math::PGRotationMatrix(tTrans._rotation));
			DirectX::XMMATRIX tTransMat = Pg::Math::PG2XM_MATRIX4X4(Pg::Math::PGTranslateMatrix(tTrans._position));

			Pg::Math::PGFLOAT3 pivot = _rendererBase3DStorage->GetRendererRotationOriginOffset();
			DirectX::XMMATRIX toPivot = DirectX::XMMatrixTranslation(-pivot.x, -pivot.y, -pivot.z);
			DirectX::XMMATRIX fromPivot = DirectX::XMMatrixTranslation(pivot.x, pivot.y, pivot.z);

			using namespace DirectX;
			tWorldTMMat = tScaleMat * toPivot * tOrigRotMat * fromPivot * tTransMat;
		}
		else
		{
			DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
			tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);
		}

		//�ٽ� Translate ���󺹱�.
		GetBaseRenderer()->_object->_transform._position = tOriginalPosValue;

		//0.01 �����ϸ� ����.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);

		//�����ϸ� ���� �Ŀ� InvTranspose�� ���ؾ���!
		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldTMMat));

		_cbFirstBase->GetDataStruct()->gCBuf_World = tWorldTMMat;
		_cbFirstBase->GetDataStruct()->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;
		_cbFirstBase->GetDataStruct()->gCBuf_ObjID = GetObjectID();
		_cbFirstBase->GetDataStruct()->gCBuf_MatID = GetMaterialID();

		//ù��° Constant Buffer���� �길 �־��ָ� �ȴ�.
		_cbFirstBase->Update();
	}

	void RenderObjectSkinnedMesh3D::UpdateSkinnedCB()
	{
		// ���� ���� ���� �����ϴ� ���� �ƴ�, ������Ʈ ������ ���� �̹� ���� �� �ִ� �����̴�.
		_cbAllSkinnedNodes->Update();
		_cbAllSkinnedBones->Update();
	}

	void RenderObjectSkinnedMesh3D::FillInNodeBuffer(const ModifiedNode_SkinnedMesh* const selfNode)
	{
		// DX���� HLSL �� �Ѿ�� �ڵ����� ��ġ�� �Ǽ� �Ѿ��.
		// HLSL ������ Row Major �ϰ� �ۼ��ϰ� �����Ƿ� �̸� ��ġ�� ���ѳ��´�.
		// �� ��ġ�� 2���ǹǷ� HLSL������ Row Major�� Matrix�� ����Ѵ�.

		if (selfNode->_index >= 0)
		{
			// nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());

			DirectX::SimpleMath::Matrix tInputData = DirectX::XMMatrixTranspose(selfNode->_relTransform->GetWorldTM());
			DirectX::SimpleMath::Matrix tZeroMat = DirectX::XMMatrixSet(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
			assert(tInputData != tZeroMat);

			//�ϴ� �������� Decompose �� �������� ���� ���� ��Ȳ.
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
		assert(_animatedModifNodeMap.contains(animNodeName) && "�� �̸��� ��ϵǾ� ���� �ʴ�!");

		const ModifiedNode_SkinnedMesh* tVal = _animatedModifNodeMap.at(animNodeName);
		return tVal->_relTransform.get();
	}

	DirectX::SimpleMath::Vector3 RenderObjectSkinnedMesh3D::FillPositionForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim)
	{
		DirectX::SimpleMath::Vector3 position;
		// Position
		{
			int positionIndex = 0;
			//Last Frame ���� �õ�, 240521
			if (renderAnim->_animAssetData->_durationTick - _currentTick < std::numeric_limits<double>::epsilon())
			{
				positionIndex = nodeAnim->_numPositionKeys - 1;
			}
			else
			{
				for (int i = 0; i < nodeAnim->_numPositionKeys; i++)
				{
					if (_currentTick < nodeAnim->_positionKeyList[i]._time)
					{
						positionIndex = i;		// i-1 < _animationTickTime < i
						break;
					}
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
		return position;
	}

	DirectX::SimpleMath::Quaternion RenderObjectSkinnedMesh3D::FillRotationForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim)
	{
		// Rotation
		DirectX::SimpleMath::Quaternion rotation;
		{
			int rotationIndex = 0;
			//Last Frame ���� �õ�, 240521
			if (renderAnim->_animAssetData->_durationTick - _currentTick < std::numeric_limits<double>::epsilon())
			{
				rotationIndex = nodeAnim->_numRotationKeys - 1;
			}
			else
			{
				for (int i = 0; i < nodeAnim->_numRotationKeys; i++)
				{
					if (_currentTick < nodeAnim->_rotationKeyList[i]._time)
					{
						rotationIndex = i;
						break;
					}
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

		return rotation;
	}

	DirectX::SimpleMath::Vector3 RenderObjectSkinnedMesh3D::FillScaleForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim)
	{
		DirectX::SimpleMath::Vector3 scale;
		// Scale�� ���� ������ ����. -> Num Scaling Keys�� ǥ���ߴ�!
			// �ϴ� ScalingKeys�� ������ 0���� Ŭ ���� ������ ��!
		if (nodeAnim->_numScalingKeys > 0)
		{
			int scaleIndex = 0;
			//Last Frame ���� �õ�, 240521
			if (renderAnim->_animAssetData->_durationTick - _currentTick < std::numeric_limits<double>::epsilon())
			{
				scaleIndex = nodeAnim->_numScalingKeys - 1;
			}
			else
			{
				for (int i = 0; i < nodeAnim->_numScalingKeys; i++)
				{
					if (_currentTick < nodeAnim->_scalingKeyList[i]._time)
					{
						scaleIndex = i;		// i-1 < _animationTickTime < i
						break;
					}
				}
			}
			if (scaleIndex == 0)
			{
				scale = nodeAnim->_scalingKeyList[0]._value;
			}
			else
			{
				double t = (_currentTick - nodeAnim->_scalingKeyList[scaleIndex - 1]._time) / (nodeAnim->_scalingKeyList[scaleIndex]._time - nodeAnim->_scalingKeyList[scaleIndex - 1]._time);
				scale = DirectX::XMVectorLerp(nodeAnim->_scalingKeyList[scaleIndex - 1]._value, nodeAnim->_scalingKeyList[scaleIndex]._value, (float)t);
			}
		}

		return scale;
	}

}

#pragma region OldSkinningCode
//void RenderObjectSkinnedMesh3D::CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	using namespace DirectX::SimpleMath;
	//	using namespace DirectX;
	//
	//	//Ű�� �Ѱ� �ۿ� ������ ������ �� ����.
	//	if (pNodeAnim->_numRotationKeys == 1)
	//	{
	//		outQuat = pNodeAnim->_rotationKeyList[0]._value;
	//		return;
	//	}
	//
	//	// ���� Rotation Ű������ ȹ��.
	//	unsigned int RotationIndex = FindRotationIndex(animTick, pNodeAnim);
	//
	//	// ���� Rotation Key Index ���� �������� üũ.
	//	unsigned int NextRotationIndex = (RotationIndex + 1);
	//	assert(NextRotationIndex < pNodeAnim->_numRotationKeys);
	//
	//	//Ű������ ���̿����� �ð� ���̸� ���Ѵ�.
	//	float DeltaTime = pNodeAnim->_rotationKeyList[NextRotationIndex]._time - pNodeAnim->_rotationKeyList[RotationIndex]._time;
	//
	//	// �ð� ��ȭ ���� ���� ���� �ð��� ���Ѵ� -> ���� ����.  
	//	float Factor = (animTick - (float)pNodeAnim->_rotationKeyList[RotationIndex]._time) / DeltaTime;
	//
	//	///���� : Factor�� 0~1 ���̷� �������� �ʴ� ���� Ȯ��!
	//	// ����-���� Ű�����ӵ��� ȸ���� ���� ���ʹϾ� �� �� ���Ѵ�.
	//	const Quaternion StartRotationQ = pNodeAnim->_rotationKeyList[RotationIndex]._value;
	//	const Quaternion EndRotationQ = pNodeAnim->_rotationKeyList[NextRotationIndex]._value;
	//
	//	// Interpolate between them using the Factor. 
	//	//Quaternion::Slerp(StartRotationQ, EndRotationQ, Factor, outQuat);
	//
	//	//�� ������ Slerp�� ���.
	//	//TRY:
	//	outQuat = MathHelper::QuaternionSlerpNoFlip(StartRotationQ, EndRotationQ, Factor);
	//
	//	// ����ȭ �� �����ڷ� ����.
	//	outQuat.Normalize();
	//}
	//
	//void RenderObjectSkinnedMesh3D::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	//Ű�� �Ѱ� �ۿ� ������ ������ �� ����.
	//	if (pNodeAnim->_numPositionKeys == 1)
	//	{
	//		outVec = pNodeAnim->_positionKeyList[0]._value;
	//		return;
	//	}
	//
	//	//������ Position �ε������� Ȯ��.
	//	unsigned int PositionIndex = FindTranslationIndex(animTick, pNodeAnim);
	//	unsigned int NextPositionIndex = (PositionIndex + 1);
	//	assert(NextPositionIndex < pNodeAnim->_numPositionKeys);
	//
	//	//Ű������ ���̿����� �ð� ���̸� ���Ѵ�.
	//	float DeltaTime = pNodeAnim->_positionKeyList[NextPositionIndex]._time - pNodeAnim->_positionKeyList[PositionIndex]._time;
	//
	//	// �ð� ��ȭ ���� ���� ���� �ð��� ���Ѵ� -> ���� ����.  
	//	float Factor = (animTick - (float)pNodeAnim->_positionKeyList[PositionIndex]._time) / DeltaTime;
	//
	//	//��/�� ������ Vector3 �� ���ϱ�.
	//	const DirectX::SimpleMath::Vector3 Start = pNodeAnim->_positionKeyList[PositionIndex]._value;
	//	const DirectX::SimpleMath::Vector3 End = pNodeAnim->_positionKeyList[NextPositionIndex]._value;
	//
	//	DirectX::SimpleMath::Vector3 Delta = End - Start;
	//
	//	//�����ڷ� ����.
	//	outVec = Start + Factor * Delta;
	//}
	//
	//unsigned int RenderObjectSkinnedMesh3D::FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	//{
	//	// Rotation Ű �����ӵ��� �ִ��� Ȯ��.
	//	assert(pNodeAnim->_numRotationKeys > 0);
	//
	//	//���� �ִϸ��̼� �ð� ������ Rotation�� ã�� �ε��� ����.
	//	for (unsigned int i = 0; i < pNodeAnim->_numRotationKeys - 1; i++)
	//	{
	//		//FIX.
	//		if (animTick < (float)pNodeAnim->_rotationKeyList[i + 1]._time)
	//		{
	//			return i;
	//		}
	//	}
	//
	//	//������� ���� ��ã�� �� == ������ ��.
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
	//	//������� ���� ��ã�� �� == ������ ��.
	//	assert(false);
	//
	//	return 0;
	//}

	//void RenderObjectSkinnedMesh3D::UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim)
	//{
	//	using namespace DirectX::SimpleMath;
	//	using namespace DirectX;

	//	Matrix tNodeTransformation = selfNode->_relTransform;

	//	//�ϴ� ���ܵα�. Single-Mesh Animation�̱⿡ ����. 
	//	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f));
	//	tNodeTransformation = rotationMatrix;

	//	// Rotation ����.
	//	Quaternion RotationQ;
	//	CalcInterpolatedRotation(RotationQ, _currentTick, nodeAnim);
	//	Matrix RotationM = DirectX::XMMatrixRotationQuaternion(RotationQ);

	//	//Translation ����.
	//	DirectX::SimpleMath::Vector3 Translation;
	//	CalcInterpolatedTranslation(Translation, _currentTick, nodeAnim);
	//	Matrix TranslationM;
	//	TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

	//	//Rotation, Translation ����.
	//	tNodeTransformation = RotationM * TranslationM;

	//	//RootNode <-> �Ϲ� Node ������ ����.
	//	Matrix tParentTransform = Matrix::Identity;
	//	if (parentNode != nullptr)
	//	{
	//		tParentTransform = parentNode->_relTransform;
	//	}

	//	//Global Transformation ã��.
	//	Matrix tGlobalTransformation = tNodeTransformation * tParentTransform;

	//	const auto& tMappedBones = _modelData->_assetSkinnedData->_mappedBones;
	//	const auto& tBoneInfoVec = _modelData->_assetSkinnedData->_renderBoneInfoVector;
	//	const auto& tMeshGlobalInvTRS = _modelData->_assetSkinnedData->_meshGlobalInverseTransform;

	//	//����ȭ������ ���� ������ ������, MappedBones ã��.
	//	if (tMappedBones.find(selfNode->_nodeName) != tMappedBones.end())
	//	{
	//		UINT tBoneIndex = tMappedBones.at(selfNode->_nodeName);

	//		// = Final Transform�� ���ұ��� ����.
	//		_boneTransformVector[tBoneIndex] =
	//			tBoneInfoVec[tBoneIndex]._boneOffset * tGlobalTransformation * tMeshGlobalInvTRS;
	//	}
	//}
#pragma endregion OldSkinningCode
