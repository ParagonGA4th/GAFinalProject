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
#include <singleton-cpp/singleton.h>
#include <algorithm>
#include <cassert>
#include <cmath>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectSkinnedMesh3D::RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID, unsigned int matID) : RenderObject3D(baseRenderer, objID, matID)
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
			int tUnmanagedTick = static_cast<int>(round((_currentAnim->_animAssetData->_ticksPerSecond) * tPassedDurSec));
			//Time 연산 실패로 값이 빠져나가는 것을 막기 위해서, CLAMP. 현재 Tick 값 투입.
			this->_currentTick = std::clamp<int>(tUnmanagedTick, 0, _currentAnim->_animAssetData->_durationTick);
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

		//RootNode는 별도로 업데이트. If문 타지 않기 위해.
		const auto& tRootNode = _modelData->_assetSkinnedData->_linearizedNodeHierarchy.at(0);
		const auto& tRootNodeAnim = _currentAnim->_animAssetData->_linearizedNodeAnimList.at(0);

		//NodeAnim이 있을 때만.
		if (tRootNodeAnim != nullptr)
		{

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

	}

	void RenderObjectSkinnedMesh3D::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{

	}

	unsigned int RenderObjectSkinnedMesh3D::FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		return NULL;
	}

	unsigned int RenderObjectSkinnedMesh3D::FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim)
	{
		return NULL;
	}


}