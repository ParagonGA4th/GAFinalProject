#include "PPFinalRenderer.h"

#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "SystemVertexShader.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include "../ParagonUtil/Log.h"

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

		

		_outlineRenderPass = std::make_unique<OutlineRenderPass>();

		//Initialize PostProcessing RenderPassses. (여기다) (모두 From-To의 양식을 따른다)
		_postprocessingRenderPassList.push_back(std::make_unique<TonemappingRenderPass>(_carrier->_quadMainRT, _carrier->_PPSwitch1));
		

		//</PostProcessing>
		

		//Final Render Pass. (From을 요구한다) -> 매개변수로.
		_finalRenderPass = std::make_unique<FinalRenderPass>(_carrier->_PPSwitch1);
	}

	void PPFinalRenderer::SetupRenderPasses()
	{
		//개별적으로 쓰일 Vertex Shader 별도로 분리.
		_ppSystemVertexShader = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/PostProcessingDefault_VS.cso", LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_finalRenderPass->Initialize();
		_outlineRenderPass->Initialize();

		for (auto& it : _postprocessingRenderPassList)
		{
			it->Initialize();
		}
	}

	unsigned int PPFinalRenderer::GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel)
	{
		//[DEPRECATED]
		//이미 Obj SRV는 ObjMat 쪽에 기록되어 있다.

		//일단 USAGE_DEFAULT에서 USAGE_STAGING으로 값을 가져온다. (CopyResource)
		_DXStorage->_deviceContext->CopyResource(_pickingStagingBuffer, _carrier->_quadObjMatRT_AoR->GetBuffer());

		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		HR(_DXStorage->_deviceContext->Map(_pickingStagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource));

		// FLOAT2 값을 가져오기.
		float* pData = static_cast<float*>(mappedResource.pData);
		float float2Value[2];

		// Assuming the uint2 value is stored in the staging buffer as a row-major structure
		//Staging Buffer를 가져온다. UINT2 Buffer가 Row-Major 기반 
		UINT rowPitch = mappedResource.RowPitch / sizeof(float);

		// HLSL 식 float2 == float 2개.
		UINT offset = (heightPixel * rowPitch) + (widthPixel * 2);

		memcpy(float2Value, pData + offset, sizeof(float) * 2);
		
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
		//일단, 임의의 값으로 맞춰놓기.
		///Processing Buffer 1로 맞춰놓음!
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
		_DXStorage->_deviceContext->ClearRenderTargetView(_carrier->_PPSwitch1->GetRTV(), _DXStorage->_backgroundColor);
		_DXStorage->_deviceContext->ClearRenderTargetView(_carrier->_PPSwitch2->GetRTV(), _DXStorage->_backgroundColor);

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

	

	
	

}