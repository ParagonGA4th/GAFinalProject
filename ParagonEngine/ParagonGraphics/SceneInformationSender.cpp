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
			//if (tDirInputCount > 0)
			//{
			//	Pg::Data::Transform* trans = &(_savedSceneInfo->_dirLightList.at(0)->_object->_transform);
			//	Pg::Math::PGFLOAT4X4 tView = Pg::Math::GetViewMatrixFromTransformValues(trans->GetRight(), trans->GetUp(), trans->GetForward(), trans->_position);
			//	Pg::Math::PGFLOAT4X4 tProj = Pg::Math::PGMatrixOrthographicLH(Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT, _savedCamData->_nearZ, _savedCamData->_farZ);
			//	//NearZ, FarZ는 Light도 똑같이 공유할 것이다. 다른 시점이라도.
			//	
			//	_cbRenderingInfo->GetDataStruct()->_lightView = PG2XM_MATRIX4X4(tView);
			//	_cbRenderingInfo->GetDataStruct()->_lightProj = PG2XM_MATRIX4X4(tProj);
			//	_cbRenderingInfo->GetDataStruct()->_lightViewProj = DirectX::XMMatrixMultiply(
			//		_cbRenderingInfo->GetDataStruct()->_lightView, _cbRenderingInfo->GetDataStruct()->_lightProj);
			//}
			//else
			//{
				//가상의 라이트를 투입한다. 이 경우, 문제는 있을 것이지만.
				//일단, 그리고 값을 활용한다. Main VP 투입.
				//																			RIGHT							UP						FORWARD
				//Pg::Math::PGFLOAT4X4 tView = Pg::Math::GetViewMatrixFromTransformValues({ 0.707107, 0.000000, 0.707107 }, { 0, 0.707107, 0.707107 }, { -0.707107, -0.707107, 0.000000 }, { 100,100,100 });
				//_cbRenderingInfo->GetDataStruct()->_lightView = PG2XM_MATRIX4X4(tView);
				
				//Custom Light View In.
				{
					using namespace DirectX;
					XMVECTOR lightDirection = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
					static float rotAmountDeg = 0.f;
					rotAmountDeg += 0.1f;
					XMMATRIX rotationMatrix = XMMatrixRotationX(XMConvertToRadians(fmod(rotAmountDeg, 360.f)));
					lightDirection = XMVector3TransformNormal(lightDirection, rotationMatrix);

					XMVECTOR lightPosition = XMVectorSet(0.0f, 1000.0f, 0.0f, 1.0f);
					XMVECTOR targetPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
					XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

					_cbRenderingInfo->GetDataStruct()->_lightView = XMMatrixLookAtLH(lightPosition, targetPosition, upVector);
				}
				
				Pg::Math::PGFLOAT4X4 tProj = Pg::Math::PGMatrixOrthographicLH(Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT, _savedCamData->_nearZ, _savedCamData->_farZ);

				_cbRenderingInfo->GetDataStruct()->_lightProj = PG2XM_MATRIX4X4(tProj);
				_cbRenderingInfo->GetDataStruct()->_lightViewProj = DirectX::XMMatrixMultiply(
					_cbRenderingInfo->GetDataStruct()->_lightView, _cbRenderingInfo->GetDataStruct()->_lightProj);
			//}
		}

		///그 전에, LightViewProj를 CamData에 옮겨주자! 맨 처음에 실행되니 문제 없이 실행될 것.
		//_carrier->_mainLightPerspectiveViewProjMatrix = _cbRenderingInfo->GetDataStruct()->_lightViewProj;
		
		//Depth 기록 디버깅을 위해, 값 정리.

		_carrier->_mainLightPerspectiveViewProjMatrix = DirectX::XMMatrixMultiply(
			Pg::Math::PG2XM_MATRIX4X4(_savedCamData->_viewMatrix), Pg::Math::PG2XM_MATRIX4X4(_savedCamData->_projMatrix));

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
