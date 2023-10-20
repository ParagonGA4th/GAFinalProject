#include "GBuffer.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"

Pg::Graphics::GBuffer::GBuffer(DXGI_FORMAT BufferFormat, DXGI_FORMAT ViewFormat)
	:_Buffer(), _RTV(), _SRV(),
	_DXStorage(LowDX11Storage::GetInstance())
{
	CreateBuffer(BufferFormat);
	CreateRTV(ViewFormat);
	CreateSRV(ViewFormat);
}

void Pg::Graphics::GBuffer::CreateBuffer(DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC tDesc;
	tDesc.Width = _DXStorage->_screenWidth;
	tDesc.Height = _DXStorage->_screenHeight;
	tDesc.MipLevels = 0;
	tDesc.ArraySize = 1;
	tDesc.Format = format;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	
	HRESULT hr = _DXStorage->_device->CreateTexture2D(&tDesc, nullptr, &_Buffer);
}

void Pg::Graphics::GBuffer::CreateRTV(DXGI_FORMAT format)
{
	D3D11_RENDER_TARGET_VIEW_DESC tDesc;
	tDesc.Format = format;
	tDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	tDesc.Texture2D.MipSlice = 0;

	HRESULT hr = _DXStorage->_device->CreateRenderTargetView(_Buffer, &tDesc, &_RTV);
}

void Pg::Graphics::GBuffer::CreateSRV(DXGI_FORMAT format)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc;
	tDesc.Format = format;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tDesc.Texture2D.MostDetailedMip = 0;
	tDesc.Texture2D.MipLevels = -1;

	HRESULT hr = _DXStorage->_device->CreateShaderResourceView(_Buffer, &tDesc, &_SRV);

}

void Pg::Graphics::GBuffer::Bind()
{
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &_RTV, _DXStorage->_depthStencilView);
}

void Pg::Graphics::GBuffer::Unbind()
{

}

void Pg::Graphics::GBuffer::Render()
{

}

ID3D11RenderTargetView* Pg::Graphics::GBuffer::GetRTV()
{
	return _RTV;
}

ID3D11ShaderResourceView* Pg::Graphics::GBuffer::GetSRV()
{
	return _SRV;
}
