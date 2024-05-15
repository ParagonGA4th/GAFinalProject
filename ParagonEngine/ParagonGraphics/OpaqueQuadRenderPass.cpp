#include "OpaqueQuadRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GeometryGenerator.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "RenderMaterial.h"
#include "DX11Headers.h"

namespace Pg::Graphics
{
	OpaqueQuadRenderPass::OpaqueQuadRenderPass(RenderMaterial* renderMat) :
		_renderMaterial(renderMat)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	OpaqueQuadRenderPass::~OpaqueQuadRenderPass()
	{

	}

	void OpaqueQuadRenderPass::Initialize()
	{
		CreateMaterialIndexConstantBuffer();
	}

	void OpaqueQuadRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//자신이 렌더할 RenderTarget을 받는다.
		//_passRenderTargetView = carrier._quadMainRT->GetRTV();
		//
		////자신이 렌더할 DepthStencil을 받는다.
		//_passDepthStencilView = carrier._dsv;
	}

	void OpaqueQuadRenderPass::BindPass()
	{
		BindVertexIndexBuffer();
		_renderMaterial->Bind();
		BindMaterialIndexConstantBuffer();

		//// Bind RenderTarget
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &_passRenderTargetView, _passDepthStencilView);
	}

	void OpaqueQuadRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void OpaqueQuadRenderPass::UnbindPass()
	{
		_renderMaterial->Unbind();
	}

	void OpaqueQuadRenderPass::ExecuteNextRenderRequirements()
	{
	
	}

	void OpaqueQuadRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//하는거 없으면, 건드리지 말아야 한다.
	}

	void OpaqueQuadRenderPass::BindVertexIndexBuffer()
	{
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	void OpaqueQuadRenderPass::BindMaterialIndexConstantBuffer()
	{
		//Mapped Subresource 메모리 클린.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_cbMatID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//자신의 Data 시작 포인터 받아오기.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material 부여.
		*(data) = _renderMaterial->GetMaterialID();

		_DXStorage->_deviceContext->Unmap(_cbMatID, 0);

		//이제 Binding. (Pixel Shader 3번 레지스터로 매핑)
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &_cbMatID);
	}

	void OpaqueQuadRenderPass::CreateMaterialIndexConstantBuffer()
	{
		//ID의 포인터를 받아온다.
		UINT* _cbData = &(_renderMaterial->GetMaterialID());
	
		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_cbMatID)));
	}

	bool OpaqueQuadRenderPass::GetIsOpaque()
	{
		//Opaque한지를 반환.
		return !_renderMaterial->GetIsUseAlphaBlending();
	}
	

}