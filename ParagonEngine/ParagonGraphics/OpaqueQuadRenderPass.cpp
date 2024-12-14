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
		//�ڽ��� ������ RenderTarget�� �޴´�.
		//_passRenderTargetView = carrier._quadMainRT->GetRTV();
		//
		////�ڽ��� ������ DepthStencil�� �޴´�.
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
		//�ϴ°� ������, �ǵ帮�� ���ƾ� �Ѵ�.
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
		//Mapped Subresource �޸� Ŭ��.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_cbMatID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//�ڽ��� Data ���� ������ �޾ƿ���.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material �ο�.
		*(data) = _renderMaterial->GetMaterialID();

		_DXStorage->_deviceContext->Unmap(_cbMatID, 0);

		//���� Binding. (Pixel Shader 3�� �������ͷ� ����)
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &_cbMatID);
	}

	void OpaqueQuadRenderPass::CreateMaterialIndexConstantBuffer()
	{
		//ID�� �����͸� �޾ƿ´�.
		UINT* _cbData = &(_renderMaterial->GetMaterialID());
	
		//Constant Buffer ��ü�� ����� �ڵ�.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec ���� 16����Ʈ ������ �� �ִ�?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // ������۴� 16����Ʈ ����
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
		//Opaque������ ��ȯ.
		return !_renderMaterial->GetIsUseAlphaBlending();
	}
	

}