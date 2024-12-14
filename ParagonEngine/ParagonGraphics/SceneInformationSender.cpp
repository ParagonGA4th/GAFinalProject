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
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/InputSystem.h"

#include <algorithm> 
#include <cmath> 
#include <limits>
#include <singleton-cpp/singleton.h>

namespace Pg::Graphics
{
	SceneInformationSender::SceneInformationSender()
	{
		_DXStorage = LowDX11Storage::GetInstance();
		_timeSystem = &singleton<Pg::Util::Time::TimeSystem>();
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
		auto& tInput = singleton<Pg::Util::Input::InputSystem>();
		if (tInput.GetKeyDown(Pg::API::Input::eKeyCode::KeyB))
		{
			_cbSceneInfo->GetDataStruct()->gCBuf_OnlyShowLMIndex = std::clamp<int>(_cbSceneInfo->GetDataStruct()->gCBuf_OnlyShowLMIndex - 1, 0, 4);
		}
		if (tInput.GetKeyDown(Pg::API::Input::eKeyCode::KeyN))
		{
			_cbSceneInfo->GetDataStruct()->gCBuf_OnlyShowLMIndex = std::clamp<int>(_cbSceneInfo->GetDataStruct()->gCBuf_OnlyShowLMIndex + 1, 0, 4);
		}

		//Constant Buffer (SceneInfo) ������Ʈ.
		_cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix = PG2XM_MATRIX4X4(_savedCamData->_viewMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_ProjMatrix = PG2XM_MATRIX4X4(_savedCamData->_projMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_ViewProjMatrix = DirectX::XMMatrixMultiply(
			_cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix, _cbSceneInfo->GetDataStruct()->gCBuf_ProjMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_InvViewMatrix = DirectX::XMMatrixInverse(nullptr, _cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix);

		_cbSceneInfo->GetDataStruct()->gCBuf_ScreenWidthHeight = { (float)Pg::Data::GameConstantData::WIDTH, (float)Pg::Data::GameConstantData::HEIGHT };

		//gCBuf_RadianTimeLoop
		_currentRecordDegrees += _timeSystem->GetDeltaTime();
		_currentRecordDegrees = fmod(_currentRecordDegrees, 360.f);
		_cbSceneInfo->GetDataStruct()->gCBuf_RadianTimeLoop = DirectX::XMConvertToRadians(_currentRecordDegrees);

		_cbSceneInfo->GetDataStruct()->gCBuf_EyePosition = PG2XM_FLOAT3(_savedCamData->_position);
		_cbSceneInfo->GetDataStruct()->gCBuf_IsSceneUseLightmap = _savedSceneInfo->_isUseLightmap;

		_cbSceneInfo->GetDataStruct()->gCBuf_isVignetteOn = _savedSceneInfo->_isVignetteOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isBloomOn = _savedSceneInfo->_isBloomOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isLutOn = _savedSceneInfo->_isLutOn;
		_cbSceneInfo->GetDataStruct()->gCBuf_isMotionBlurOn = _savedSceneInfo->_isMotionBlurOn;

		//������Ʈ.
		_cbSceneInfo->Update();
		//SceneInfo Constant Buffer Bind -> ���߿� CBCarrier�� �̷� Ŭ���� �־�� �� ���̴�.
		//VS / PS ��� �� �� �ְ�!
		_cbSceneInfo->BindVS(4);
		_cbSceneInfo->BindPS(4);
	}

	void SceneInformationSender::ProcessLightInfoData()
	{
		//Camera Near / Far ����.
		_cbRenderingInfo->GetDataStruct()->_Camera_NearPlane = _savedCamData->_nearZ;
		_cbRenderingInfo->GetDataStruct()->_Camera_FarPlane = _savedCamData->_farZ;

		//Light ��ü ���� �Űܴ��.

		unsigned int tDirInputCount = std::min((unsigned int)_savedSceneInfo->_dirLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);
		unsigned int tSpotInputCount = std::min((unsigned int)_savedSceneInfo->_spotLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);
		unsigned int tPointInputCount = std::min((unsigned int)_savedSceneInfo->_pointLightList.size(), (unsigned int)SceneInformationList::LIGHT_MAX_GPU_PASS_COUNT);

		//������ �־�� �ϰ� �ϱ�! ��������� �־������� �ʴ´�.
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


		//Shadow�� ���ϱ� ���� ��� ��ȯ.
		//float tLightSeparatedDistance = 100.f;
		float tLightSeparatedDistance = 200.f;
		Pg::Math::PGFLOAT3 tForwardDir;
		float tMainLightRadiance;
		tForwardDir = { 0.f, -1.f, 0.f };
		tMainLightRadiance = 1.0f;

		//Scene�� ����Ű�� �߾� �����̶� ����� ������ �־�� �Ѵ�.
		using namespace DirectX;
		//Pg::Math::PGFLOAT3 tNewPosition = { 0.f, 0.f, 0.0f };
		Pg::Math::PGFLOAT3 tCamPosition = _savedCamData->_position;
		Pg::Math::PGFLOAT3 tPlyPosition = _savedCamData->_playerPosition;
		//Pg::Math::PGFLOAT3 tHeadedVector = PGFloat3Normalize(tCamPosition - tPlyPosition);
		//Pg::Math::PGFLOAT3 tHeadedVector = PGFloat3Normalize(tPlyPosition - tCamPosition);
		//Pg::Math::PGFLOAT3 tNewPosition = tPlyPosition += (tHeadedVector * 10);
		Pg::Math::PGFLOAT3 tNewPosition = tPlyPosition;
		tNewPosition.y += 20.f;
		//Pg::Math::PGFLOAT3 tCameraForwardVec = Pg::Math::PGFloat3Normalize(Pg::Math::GetForwardVectorFromQuat(_savedCamData->_rotation));
		//tNewPosition.x -= (tCameraForwardVec.x * 20.0f);
		//tNewPosition.z -= (tCameraForwardVec.z * 20.0f);

		// �ϴ��� Direction ��ü�� ������ �ȵǾ� ���� ���� �ִ�. ���� �׷� ��� -> Direction ��ü�� LightComponent���� ���ְ�, Transform Quaternion���� �Ź� ��ȯ�ϰ� �ؾ� �Ѵ�.
		Pg::Math::PGFLOAT3 tLightPos = tNewPosition + (-tForwardDir) * tLightSeparatedDistance;
		XMVECTOR lightPosition = PG2XM_FLOAT3_VECTOR(tLightPos);
		//LightPosition���� NewPosition�� �ٶ󺸸� �� ��.
		XMVECTOR targetPosition = PG2XM_FLOAT3_VECTOR(tNewPosition);
		XMVECTOR upVector = PG2XM_FLOAT3_VECTOR(Pg::Math::PGFLOAT3::GlobalUp());

		//Light�� World�� ���ϱ�.
		{
			XMVECTOR tLightWorldPosition = lightPosition;
			XMVECTOR tLightQuat = XMQuaternionRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 1.0f), XMConvertToRadians(90.f));
			XMVECTOR tLightScale = XMVectorSet(1.f, 1.f, 1.f, 1.f);
			XMMATRIX tLightWorldMatrix = XMMatrixAffineTransformation(tLightScale, XMVectorZero(), tLightQuat, tLightWorldPosition);
			XMMATRIX tLightViewMatrix = XMMatrixInverse(nullptr, tLightWorldMatrix);
			_cbRenderingInfo->GetDataStruct()->_lightView = tLightViewMatrix;
		}

		//LightView�� �ٸ��� �ִ� �Ͱ� ����,
		//�߰��� MainLightDir / Radiance�� �Űܾ� �Ѵ�.
		_cbRenderingInfo->GetDataStruct()->_indep_MainLightDir = PG2XM_FLOAT3(tForwardDir);
		_cbRenderingInfo->GetDataStruct()->_indep_MainLightRadiance = tMainLightRadiance;

		//float width = 35.0f * 2.0f;  // right - left
		//float height = 35.0f * 2.0f; // top - bottom

		float width = 20.0f * 2.0f;  // right - left
		float height = 20.0f * 2.0f; // top - bottom
		Pg::Math::PGFLOAT4X4 tProj = Pg::Math::PGMatrixOrthographicLH(width,
			height, _savedCamData->_nearZ, _savedCamData->_farZ);

		_cbRenderingInfo->GetDataStruct()->_lightProj = PG2XM_MATRIX4X4(tProj);
		_cbRenderingInfo->GetDataStruct()->_lightViewProj = DirectX::XMMatrixMultiply(
			_cbRenderingInfo->GetDataStruct()->_lightView, _cbRenderingInfo->GetDataStruct()->_lightProj);


		///�� ����, LightViewProj�� CamData�� �Ű�����! �� ó���� ����Ǵ� ���� ���� ����� ��.
		_carrier->_mainLightPerspectiveViewProjMatrix = _cbRenderingInfo->GetDataStruct()->_lightViewProj;

		//������ �������, ������ ������Ʈ�ؾ�.
		//������Ʈ.
		_cbRenderingInfo->Update();
		//LightInfo Constant Buffer Bind -> ���߿� CBCarrier�� �̷� Ŭ���� �־�� �� ���̴�.
		_cbRenderingInfo->BindVS(5); //VS / PS ��� ������Ʈ.
		_cbRenderingInfo->BindPS(5);
	}

	void SceneInformationSender::ReceiveData(SceneInformationList& info)
	{

	}

	void SceneInformationSender::CreateBuffers()
	{
		_cbSceneInfo = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>>();
		_cbRenderingInfo = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbRenderingInfo>>();

		_cbSceneInfo->GetDataStruct()->gCBuf_OnlyShowLMIndex = 0;
	}


}
