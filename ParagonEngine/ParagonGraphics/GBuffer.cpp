#include "GBuffer.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"

Pg::Graphics::GBuffer::GBuffer()
	:_Buffer(), _RTV(), _SRV(),
	_DXStorage(LowDX11Storage::GetInstance())
{

}

void Pg::Graphics::GBuffer::CreateBuffer()
{
	D3D11_TEXTURE2D_DESC tDesc;
	tDesc.Width = _DXStorage->_screenWidth;
	tDesc.Height = _DXStorage->_screenHeight;
	tDesc.MipLevels = 0;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	
	_DXStorage->_device->CreateTexture2D(&tDesc, nullptr, &_Buffer);
}

void Pg::Graphics::GBuffer::CreateRTV()
{
	D3D11_RENDER_TARGET_VIEW_DESC tDesc;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	_DXStorage->_device->CreateRenderTargetView(_Buffer, &tDesc, &_RTV);
}

void Pg::Graphics::GBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	_DXStorage->_device->CreateShaderResourceView(_Buffer, &tDesc, &_SRV);

}

void Pg::Graphics::GBuffer::Bind()
{
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &_RTV, _DXStorage->_depthStencilView);
}

void Pg::Graphics::GBuffer::Unbind()
{

}
