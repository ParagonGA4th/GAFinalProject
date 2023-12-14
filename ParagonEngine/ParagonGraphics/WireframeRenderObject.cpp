#include "WireframeRenderObject.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MathHelper.h"
#include "LayoutDefine.h"

#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	WireframeRenderObject::WireframeRenderObject() :
		_DXStorage(Pg::Graphics::LowDX11Storage::GetInstance()), _VB(nullptr), _IB(nullptr)
	{
		CreateConstantBuffers();
	}

	WireframeRenderObject::~WireframeRenderObject()
	{

	}

	void WireframeRenderObject::UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		DirectX::XMFLOAT4X4 tWorldTM;
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMStoreFloat4x4(&(_cbData.worldMatrix), tWorldTMMat);
		DirectX::XMStoreFloat4x4(&(_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		_DXStorage->_deviceContext->UpdateSubresource(_cBuffer, 0, NULL, &_cbData, 0, 0);
	}

	void WireframeRenderObject::BindConstantBuffers()
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &_cBuffer);
	}

	void WireframeRenderObject::Render()
	{
		UINT stride = sizeof(LayoutDefine::VinWireframePrimitive);
		UINT offset = 0;

		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &_VB, &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(_IB, DXGI_FORMAT_R32_UINT, 0);

		_DXStorage->_deviceContext->DrawIndexed(_indexSize, 0, 0);
	}

	void WireframeRenderObject::UnbindConstantBuffers()
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &_nullBuffer);
	}

	void WireframeRenderObject::CreateConstantBuffers()
	{
		int sizeCB = (((sizeof(WireframeRenderObject::CB) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);
		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tCBufferDesc.MiscFlags = 0;

		_cbSubresData.pSysMem = &_cbData;

		HR(_DXStorage->_device->CreateBuffer(&tCBufferDesc, &_cbSubresData, &_cBuffer));
	}
}
