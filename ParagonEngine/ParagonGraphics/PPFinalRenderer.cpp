#include "PPFinalRenderer.h"

#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "SystemVertexShader.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"
#include "../ParagonData/ParagonDefines.h"

#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Graphics
{
	PPFinalRenderer::PPFinalRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	void PPFinalRenderer::Initialize()
	{
		CreateStagingPickingBuffer();
		InitPostProcessingQuads();
		CreateDebugOverlayQuads();

		_outlineRenderPass = std::make_unique<OutlineRenderPass>();

		//Initialize PostProcessing RenderPassses. (여기다) (모두 From-To의 양식을 따른다)
		_postprocessingRenderPassList.push_back(std::make_unique<HeightFogRenderPass>(_carrier->_quadMainRT, _carrier->_PPSwitch1));
		_postprocessingRenderPassList.push_back(std::make_unique<VignetteRenderPass>(_carrier->_PPSwitch1, _carrier->_PPSwitch2));
		_postprocessingRenderPassList.push_back(std::make_unique<BloomRenderPass>(_carrier->_PPSwitch2, _carrier->_PPSwitch1));
		_postprocessingRenderPassList.push_back(std::make_unique<LUTRenderPass>(_carrier->_PPSwitch1, _carrier->_PPSwitch2));

		//중간에 TimeSystem 받아오기.
		_timeSystem = &singleton<Pg::Util::Time::TimeSystem>();


		//</PostProcessing>

		//FadeInOut
		_fadeInOutPass = std::make_unique<FadeInOutPass>();

		//Final Render Pass. (From을 요구한다) -> 매개변수로.
		_finalRenderPass = std::make_unique<FinalRenderPass>();
	}

	void PPFinalRenderer::SetupRenderPasses()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;

		//개별적으로 쓰일 Vertex Shader 별도로 분리.
		_ppSystemVertexShader = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(POSTPROCESSING_DEFAULT_VS_DIRECTORY), LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_finalRenderPass->Initialize();
		_outlineRenderPass->Initialize();

		for (auto& it : _postprocessingRenderPassList)
		{
			it->Initialize();
		}

		_fadeInOutPass->Initialize();
	}

	unsigned int PPFinalRenderer::GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel)
	{
		//[DEPRECATED]
		//이미 Obj SRV는 ObjMat 쪽에 기록되어 있다.

		//일단 USAGE_DEFAULT에서 USAGE_STAGING으로 값을 가져온다. (CopyResource)
		//ObjMatAoR은 원래에서 5번째 렌더타겟.
		_DXStorage->_deviceContext->CopyResource(_pickingStagingBuffer, _carrier->_gBufRequiredInfoRT.at(5)->GetBuffer());

		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		HR(_DXStorage->_deviceContext->Map(_pickingStagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource));

		// FLOAT2 값을 가져오기.
		float* pData = static_cast<float*>(mappedResource.pData);
		float float2Value[4];

		// Assuming the uint2 value is stored in the staging buffer as a row-major structure
		//Staging Buffer를 가져온다. UINT2 Buffer가 Row-Major 기반 
		UINT rowPitch = mappedResource.RowPitch / sizeof(float);

		// HLSL 식 float2 == float 2개.
		UINT offset = (heightPixel * rowPitch) + (widthPixel * 4);

		memcpy(float2Value, pData + offset, sizeof(float) * 4);

		// Unmap the staging buffer
		_DXStorage->_deviceContext->Unmap(_pickingStagingBuffer, 0);

		//ObjMat 순서이니, 0번째 인덱스가 Object를 따름.
		return float2Value[0];
	}

	void PPFinalRenderer::RenderOutlineStencil(Pg::Data::CameraData* camData)
	{
		if (_outlineRenderingMode)
		{
			//안에서만 렌더링.
			_outlineRenderPass->SetOutlinePlannedObjectID(_pickedObjID);
			_outlineRenderPass->ReceiveRequiredElements(*_carrier);
			_outlineRenderPass->BindPass();
			_outlineRenderPass->RenderPass(nullptr, camData);
			_outlineRenderPass->UnbindPass();
			_outlineRenderPass->ExecuteNextRenderRequirements();
			_outlineRenderPass->PassNextRequirements(*_carrier);
		}
	}

	void PPFinalRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		////SRV Index 0 : Opaque Quad의 인덱스 전달.
		//_carrier->_srvArray[0] = _opaqueQuadRTV->GetSRV();
		////SRV Index 1 : ObjMat 레이어에서 나온 뎁스의 SRV 전달.
		//_carrier->_srvArray[1] = _objMatDepthSRV;

		//Final Render Pass.
		_finalRenderPass->ReceiveRequiredElements(*_carrier);
		_finalRenderPass->BindPass();
		_finalRenderPass->RenderPass(renderObjectList, camData);
		_finalRenderPass->UnbindPass();
		_finalRenderPass->ExecuteNextRenderRequirements();
		_finalRenderPass->PassNextRequirements(*_carrier);
	}

	void PPFinalRenderer::ConfirmCarrierData()
	{

	}

	void PPFinalRenderer::InitPostProcessingQuads()
	{
		//요구되는 렌더 리소스 만들기 (GBufferRender & Depth Stencil)
		_postProcessingBuffer1 = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_postProcessingBuffer2 = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_carrier->_PPSwitch1 = _postProcessingBuffer1.get();
		_carrier->_PPSwitch2 = _postProcessingBuffer2.get();

		//만약 PostProcessing Stage가 없으면, Editor에서 받을 SRV가 없을 수도. 
		//최종 PostProcessing Stage로 설정하자 -> 이건 개별 Pass에서.
		_carrier->_toSendSRVToEngine = _carrier->_PPSwitch1->GetSRV();
	}

	void PPFinalRenderer::CreateStagingPickingBuffer()
	{
		D3D11_TEXTURE2D_DESC stagingBufferDesc;
		ZeroMemory(&stagingBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		stagingBufferDesc.Width = LowDX11Storage::GetInstance()->_screenWidth;
		stagingBufferDesc.Height = LowDX11Storage::GetInstance()->_screenHeight;
		stagingBufferDesc.MipLevels = 0;
		stagingBufferDesc.ArraySize = 1;
		stagingBufferDesc.Format = DXGI_FORMAT_R32G32_FLOAT; // Adjust the format as per your needs
		stagingBufferDesc.SampleDesc.Count = 1;
		stagingBufferDesc.SampleDesc.Quality = 0;
		stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
		stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		HR(_DXStorage->_device->CreateTexture2D(&stagingBufferDesc, nullptr, &_pickingStagingBuffer));
	}

	void PPFinalRenderer::SetOutlineRenderingMode(bool val)
	{
		_outlineRenderingMode = val;
	}

	void PPFinalRenderer::SetObjectIDSelected(unsigned int val)
	{
		if (val == 0)
		{
			return;
		}

		_pickedObjID = val;
	}

	void PPFinalRenderer::RenderPostProcessingStages(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//미리 RenderTarget Clear. Depth Buffer는 바인딩 자체가 안 될 것이니, Clear 필요 X.
		//이제 개별적으로 클리어 필요할 것.
		_DXStorage->_deviceContext->ClearRenderTargetView(_carrier->_PPSwitch1->GetRTV(), _DXStorage->_backgroundColor);
		_DXStorage->_deviceContext->ClearRenderTargetView(_carrier->_PPSwitch2->GetRTV(), _DXStorage->_backgroundColor);
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		//Default Quad Vertex Shader Bind.
		_ppSystemVertexShader->Bind();

		for (auto& it : _postprocessingRenderPassList)
		{
			it->ReceiveRequiredElements(*_carrier);
			it->BindPass();
			it->RenderPass(renderObjectList, camData);
			it->UnbindPass();
			it->ExecuteNextRenderRequirements();
			it->PassNextRequirements(*_carrier);
		}

		//Default Quad Vertex Shader Unbind.
		_ppSystemVertexShader->Unbind();


	}

	void PPFinalRenderer::ConnectDefaultResources()
	{
		for (auto& it : _postprocessingRenderPassList)
		{
			it->ConnectDefaultResources();
		}
	}

	void PPFinalRenderer::Effect_FadeIn()
	{
		_isFadingIn = true;
		_isFadingOut = false;

		//FadeIn Logic 그냥 안에서 실행.
		//while (_isFadingIn)
		//{
		//	PG_WARN("FADING_IN");
		//	//끝날때까지.
		//	WhenActivatedUpdateFadeLogic();
		//}
	}

	void PPFinalRenderer::Effect_FadeOut()
	{
		_isFadingOut = true;
		_isFadingIn = false;

		//while (_isFadingOut)
		//{
		//	PG_WARN("FADING_OUT");
		//	//끝날때까지.
		//	WhenActivatedUpdateFadeLogic();
		//}
	}

	void PPFinalRenderer::WhenActivatedUpdateFadeLogic()
	{
		//float dt = _timeSystem->GetDeltaTime();
		float dt = 0.01f;
		if (_isFadingIn)
		{
			_fadeEffectSourceRatio += dt;
			if (_fadeEffectSourceRatio >= 1.0f)
			{
				_fadeEffectSourceRatio = 1.0f;
				_isFadingIn = false;
			}
		}
		else if (_isFadingOut)
		{
			_fadeEffectSourceRatio -= dt;
			if (_fadeEffectSourceRatio <= 0.f)
			{
				_fadeEffectSourceRatio = 0.f;
				_isFadingOut = false;
			}
		}

		//0-1로 한정. 필요 없겠지만 추가로 작성.
		_fadeEffectSourceRatio = std::clamp<float>(_fadeEffectSourceRatio, 0.f, 1.f);
	}

	void PPFinalRenderer::RenderFadeInOut()
	{
		//FadeIn-Out 관리.	
		{
			WhenActivatedUpdateFadeLogic();

			_carrier->_toSendSRVToEngine = _carrier->_PPSwitch1->GetSRV();
			_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_PPSwitch1->GetRTV()), nullptr);
			_fadeInOutPass->Render(_fadeEffectSourceRatio, _carrier, _carrier->_PPSwitch2->GetSRV());
		}
	}

	void PPFinalRenderer::CreateDebugOverlayQuads()
	{
//#if defined(DEBUG) | defined(_DEBUG)
//		D3D11_VIEWPORT viewport;
//		viewport.TopLeftX = 100.0f;
//		viewport.TopLeftY = 100.0f;
//		viewport.Width = 400.0f;
//		viewport.Height = 400.0f;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		
//		//DirectX::XMFLOAT2 widthHeight = { Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT };
//		DirectX::XMFLOAT2 widthHeight = { 300, 300 };
//		_debugOverlayShadowQuad = std::make_unique<DebugOverlayQuad>(
//			Pg::Defines::DEBUG_SHADOW_QUAD_PS_DIRECTORY, viewport, widthHeight);
//#endif
	}

	void PPFinalRenderer::RenderDebugQuadsOverlay()
	{
//#if defined(DEBUG) | defined(_DEBUG)
//		_debugOverlayShadowQuad->Render();
//#endif
	}

}