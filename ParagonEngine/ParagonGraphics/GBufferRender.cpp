#include "GBufferRender.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"

Pg::Graphics::GBufferRender::GBufferRender(DXGI_FORMAT BufferFormat, DXGI_FORMAT ViewFormat, DirectX::XMFLOAT2 widthHeight /*= {Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT}*/)
	: _Buffer(), _RTV(), _SRV(), _DXStorage(LowDX11Storage::GetInstance()), _widthHeight(widthHeight)
{
	CreateBuffer(BufferFormat);
	CreateRTV(ViewFormat);
	CreateSRV(ViewFormat);
}

Pg::Graphics::GBufferRender::~GBufferRender()
{
	//
}

void Pg::Graphics::GBufferRender::CreateBuffer(DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC tDesc;
	tDesc.Width = _widthHeight.x;
	tDesc.Height = _widthHeight.y;
	tDesc.MipLevels = 0;
	tDesc.ArraySize = 1;
	tDesc.Format = format;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	HR(_DXStorage->_device->CreateTexture2D(&tDesc, nullptr, &_Buffer));
}

void Pg::Graphics::GBufferRender::CreateRTV(DXGI_FORMAT format)
{
	D3D11_RENDER_TARGET_VIEW_DESC tDesc;
	tDesc.Format = format;
	tDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	tDesc.Texture2D.MipSlice = 0;

	HR(_DXStorage->_device->CreateRenderTargetView(_Buffer, &tDesc, &_RTV));
}

void Pg::Graphics::GBufferRender::CreateSRV(DXGI_FORMAT format)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc;
	tDesc.Format = format;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tDesc.Texture2D.MostDetailedMip = 0;
	tDesc.Texture2D.MipLevels = -1;

	HR(_DXStorage->_device->CreateShaderResourceView(_Buffer, &tDesc, &_SRV));
}

ID3D11RenderTargetView*& Pg::Graphics::GBufferRender::GetRTV()
{
	return _RTV;
}

ID3D11ShaderResourceView*& Pg::Graphics::GBufferRender::GetSRV()
{
	return _SRV;
}

ID3D11Texture2D*& Pg::Graphics::GBufferRender::GetBuffer()
{
	return _Buffer;
}

