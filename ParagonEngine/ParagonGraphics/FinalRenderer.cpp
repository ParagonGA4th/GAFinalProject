#include "FinalRenderer.h"

#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"



#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include "../ParagonUtil/Log.h"

namespace Pg::Graphics
{
	FinalRenderer::FinalRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	void FinalRenderer::Initialize()
	{
		CreateStagingPickingBuffer();

		_outlineRenderPass = std::make_unique<OutlineRenderPass>();
		_finalRenderPass = std::make_unique<FinalRenderPass>();
	}

	void FinalRenderer::SetupRenderPasses()
	{
		_finalRenderPass->Initialize();
		_outlineRenderPass->Initialize();
	}

	unsigned int FinalRenderer::GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel)
	{
		//[DEPRECATED]
		//이미 Obj SRV는 ObjMat 쪽에 기록되어 있다.

		//일단 USAGE_DEFAULT에서 USAGE_STAGING으로 값을 가져온다. (CopyResource)
		_DXStorage->_deviceContext->CopyResource(_pickingStagingBuffer, _carrier->_quadObjMatRT->GetBuffer());

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

	void FinalRenderer::RenderOutlineStencil(Pg::Data::CameraData* camData)
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

	void FinalRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
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

	void FinalRenderer::ConfirmCarrierData()
	{

	}

	void FinalRenderer::CreateStagingPickingBuffer()
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

	void FinalRenderer::SetOutlineRenderingMode(bool val)
	{
		_outlineRenderingMode = val;
	}

	void FinalRenderer::SetObjectIDSelected(unsigned int val)
	{
		_pickedObjID = val;
	}
	

}