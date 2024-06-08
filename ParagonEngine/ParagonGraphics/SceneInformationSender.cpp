#include "SceneInformationSender.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "D3DCarrier.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameConstantData.h"

#include <algorithm> 
#include <cmath> 

namespace Pg::Graphics
{
	SceneInformationSender::SceneInformationSender()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	SceneInformationSender::~SceneInformationSender()
	{

	}

	void SceneInformationSender::Initialize()
	{
		CreateBuffers();
	}

	void SceneInformationSender::SendData(D3DCarrier* carrier, const SceneInformationList& info, const Pg::Data::CameraData* const camData)
	{
		_carrier = carrier;
		_savedSceneInfo = &info;
		_savedCamData = camData;
	}

	void SceneInformationSender::ProcessData()
	{
		ProcessSceneInfoData();
		ProcessLightInfoData();
	}

	void SceneInformationSender::ProcessSceneInfoData()
	{
		//Constant Buffer (SceneInfo) 업데이트.
		_cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix = PG2XM_MATRIX4X4(_savedCamData->_viewMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_ProjMatrix = PG2XM_MATRIX4X4(_savedCamData->_projMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_ViewProjMatrix = DirectX::XMMatrixMultiply(
			_cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix, _cbSceneInfo->GetDataStruct()->gCBuf_ProjMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_InvViewMatrix = DirectX::XMMatrixInverse(nullptr, _cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix);

		_cbSceneInfo->GetDataStruct()->gCBuf_ScreenWidthHeight = { (float)Pg::Data::GameConstantData::WIDTH, (float)Pg::Data::GameConstantData::HEIGHT };
		_cbSceneInfo->GetDataStruct()->gCBuf_EyePosition = PG2XM_FLOAT3(_savedCamData->_position);
		_cbSceneInfo->GetDataStruct()->gCBuf_IsSceneUseLightmap = _savedSceneInfo->_isUseLightmap;

		_cbSceneInfo->GetDataStruct()->gCBuf_isVignetteOn = _savedSceneInfo->_isVignetteOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isBloomOn = _savedSceneInfo->_isBloomOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isLutOn = _savedSceneInfo->_isLutOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isMotionBlurOn = _savedSceneInfo->_isMotionBlurOn;

		//업데이트.
		_cbSceneInfo->Update();
		//SceneInfo Constant Buffer Bind -> 나중에 CBCarrier뭐 이런 클래스 있어야 할 것이다.
		//VS / PS 모두 쓸 수 있게!
		_cbSceneInfo->BindVS(4);
		_cbSceneInfo->BindPS(4);
	}

	void SceneInformationSender::ProcessLightInfoData()
	{
		//Camera Near / Far 전달.
		_cbRenderingInfo->GetDataStruct()->_Camera_NearPlane = _savedCamData->_nearZ;
		_cbRenderingInfo->GetDataStruct()->_Camera_FarPlane = _savedCamData->_farZ;

		//Light 자체 정보 옮겨담기.

		unsigned int tDirInputCount = std::min((unsigned int)_savedSceneInfo->_dirLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);
		unsigned int tSpotInputCount = std::min((unsigned int)_savedSceneInfo->_spotLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);
		unsigned int tPointInputCount = std::min((unsigned int)_savedSceneInfo->_pointLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);

		//무조건 있어야 하게 하기! 명시적으로 넣어주지는 않는다.
		for (int i = 0; i < tDirInputCount; i++)
		{
			_cbRenderingInfo->GetDataStruct()->_dirLightArray[i] = DirLightGPU(_savedSceneInfo->_dirLightList.at(i));
		}
		_cbRenderingInfo->GetDataStruct()->_dirLightCount = tDirInputCount;

		for (int i = 0; i < tSpotInputCount; i++)
		{
			_cbRenderingInfo->GetDataStruct()->_spotLightArray[i] = SpotLightGPU(_savedSceneInfo->_spotLightList.at(i));
		}
		_cbRenderingInfo->GetDataStruct()->_spotLightCount = tSpotInputCount;

		for (int i = 0; i < tPointInputCount; i++)
		{
			_cbRenderingInfo->GetDataStruct()->_pointLightArray[i] = PointLightGPU(_savedSceneInfo->_pointLightList.at(i));
		}
		_cbRenderingInfo->GetDataStruct()->_pointLightCount = tPointInputCount;


		//Shadow를 구하기 위한 행렬 반환.
		{
			//																			RIGHT							UP						FORWARD
			//Pg::Math::PGFLOAT4X4 tView = Pg::Math::GetViewMatrixFromTransformValues({ 0.707107, 0.000000, 0.707107 }, { 0, 0.707107, 0.707107 }, { -0.707107, -0.707107, 0.000000 }, { 100,100,

			float tLightSeparatedDistance = 50.f;
			Pg::Math::PGFLOAT3 tForwardDir;
			float tMainLightRadiance;
			if (tDirInputCount > 0)
			{
				tForwardDir = Pg::Math::GetForwardVectorFromQuat(_savedSceneInfo->_dirLightList.at(0)->_object->_transform._rotation);
				tMainLightRadiance = _savedSceneInfo->_dirLightList.at(0)->_radiance;
			}
			else
			{
				tForwardDir = { -0.707107f, -0.707107f, 0.f };
				tMainLightRadiance = 1.0f;
			}

			//Scene이 가리키는 중앙 지점이랑 가까운 지점에 있어야 한다.
			using namespace DirectX;
			Pg::Math::PGFLOAT3 tNewPosition = _savedCamData->_position;
			// 일단이 Direction 자체가 연동이 안되어 있을 수도 있다. 만약 그럴 경우 -> Direction 자체를 LightComponent에서 없애고, Transform Quaternion에서 매번 변환하게 해야 한다.
			Pg::Math::PGFLOAT3 tLightPos = tNewPosition + (-tForwardDir) * tLightSeparatedDistance;
			XMVECTOR lightPosition = PG2XM_FLOAT3_VECTOR(tLightPos);
			//LightPosition에서 NewPosition을 바라보면 될 것.
			XMVECTOR targetPosition = PG2XM_FLOAT3_VECTOR(tNewPosition);
			XMVECTOR upVector = PG2XM_FLOAT3_VECTOR(Pg::Math::PGFLOAT3::GlobalUp());
			_cbRenderingInfo->GetDataStruct()->_lightView = XMMatrixLookAtLH(lightPosition, targetPosition, upVector);

			//LightView를 다르게 넣는 것과 더해,
			//추가된 MainLightDir / Radiance를 옮겨야 한다.
			_cbRenderingInfo->GetDataStruct()->_indep_MainLightDir = PG2XM_FLOAT3(tForwardDir);
			_cbRenderingInfo->GetDataStruct()->_indep_MainLightRadiance = tMainLightRadiance;

			Pg::Math::PGFLOAT4X4 tProj = Pg::Math::PGMatrixOrthographicLH(Pg::Data::GameConstantData::SHADOW_MAP_LENGTH,
				Pg::Data::GameConstantData::SHADOW_MAP_LENGTH, _savedCamData->_nearZ, _savedCamData->_farZ);

			_cbRenderingInfo->GetDataStruct()->_lightProj = PG2XM_MATRIX4X4(tProj);
			_cbRenderingInfo->GetDataStruct()->_lightViewProj = DirectX::XMMatrixMultiply(
				_cbRenderingInfo->GetDataStruct()->_lightView, _cbRenderingInfo->GetDataStruct()->_lightProj);
		}

		///그 전에, LightViewProj를 CamData에 옮겨주자! 맨 처음에 실행되니 문제 없이 실행될 것.
		_carrier->_mainLightPerspectiveViewProjMatrix = _cbRenderingInfo->GetDataStruct()->_lightViewProj;

		//정보를 담았으니, 이제는 업데이트해야.
		//업데이트.
		_cbRenderingInfo->Update();
		//LightInfo Constant Buffer Bind -> 나중에 CBCarrier뭐 이런 클래스 있어야 할 것이다.
		_cbRenderingInfo->BindVS(5); //VS / PS 모두 업데이트.
		_cbRenderingInfo->BindPS(5);
	}

	void SceneInformationSender::ReceiveData(SceneInformationList& info)
	{

	}

	void SceneInformationSender::CreateBuffers()
	{
		_cbSceneInfo = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>>();
		_cbRenderingInfo = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbRenderingInfo>>();
	}


}
