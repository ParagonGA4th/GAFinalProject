#include "GBufferDepthStencil.h"
#include "DX11Headers.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	GBufferDepthStencil::GBufferDepthStencil(D3D11_DEPTH_STENCIL_DESC* dsDesc, DirectX::XMFLOAT2 widthHeight)
		:_Buffer(), _DSV(), _SRV(),
		_DXStorage(LowDX11Storage::GetInstance()), _widthHeight(widthHeight)
	{
		//Default.
		//DXGI_FORMAT_R32_TYPELESS -> Buffer Format
		//DXGI_FORMAT_D32_FLOAT_S8X24_UINT -> DepthStencilView Format.
		//가능한 이유 : Depth가 24비트(Float), Stencil(uint)이 32비트 중 8비트 활용해서 기록.

		DXGI_FORMAT tBufferformat = DXGI_FORMAT_R24G8_TYPELESS;
		DXGI_FORMAT tDSVformat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DXGI_FORMAT tSRVformat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		CreateBuffer(tBufferformat);
		CreateDSV(tDSVformat);
		CreateSRV(tSRVformat);
		CreateDepthStencilState(dsDesc);
	}

	GBufferDepthStencil::~GBufferDepthStencil()
	{
		//
	}

	void GBufferDepthStencil::CreateBuffer(DXGI_FORMAT format)
	{
		D3D11_TEXTURE2D_DESC tDesc;

		tDesc.Width = _widthHeight.x;
		tDesc.Height = _widthHeight.y;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = 1; // 0이 아님.
		tDesc.Format = format;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		tDesc.CPUAccessFlags = 0;
		tDesc.MiscFlags = 0;

		HR(_DXStorage->_device->CreateTexture2D(&tDesc, nullptr, &_Buffer));
	}

	void GBufferDepthStencil::CreateDSV(DXGI_FORMAT format)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		//Depth Stencil 
		descDSV.Flags = 0;
		descDSV.Format = format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		HR(_DXStorage->_device->CreateDepthStencilView(_Buffer, &descDSV, &_DSV));
	}

	void GBufferDepthStencil::CreateSRV(DXGI_FORMAT format)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC tDesc;
		tDesc.Format = format;
		tDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		tDesc.Texture2D.MostDetailedMip = 0;
		tDesc.Texture2D.MipLevels = -1;

		HR(_DXStorage->_device->CreateShaderResourceView(_Buffer, &tDesc, &_SRV));
	}

	void GBufferDepthStencil::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dsDesc)
	{
		if (dsDesc == nullptr)
		{
			//일단은 Default로 만든다. 
			D3D11_DEPTH_STENCIL_DESC tDepthStencilDesc;
			tDepthStencilDesc.DepthEnable = TRUE;
			tDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			tDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			tDepthStencilDesc.StencilEnable = FALSE;
			tDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			tDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			tDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			tDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			tDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			tDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			tDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			tDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			tDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			tDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

			HR(_DXStorage->_device->CreateDepthStencilState(&tDepthStencilDesc, &_dsState));
		}
		else
		{
			HR(_DXStorage->_device->CreateDepthStencilState(dsDesc, &_dsState));
		}
	}

	ID3D11DepthStencilView*& GBufferDepthStencil::GetDSV()
	{
		return _DSV;
	}

	ID3D11ShaderResourceView*& GBufferDepthStencil::GetSRV()
	{
		return _SRV;
	}

	ID3D11DepthStencilState*& GBufferDepthStencil::GetDSState()
	{
		return _dsState;
	}
	

}
