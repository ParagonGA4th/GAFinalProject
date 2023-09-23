#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

#include "RenderShader.h"
#include "DX11Headers.h"

#include <vector>
#include <string>

namespace Pg::Graphics
{
	LowDX11Logic::LowDX11Logic()
		:_DXStorage(LowDX11Storage::GetInstance()),
		hr(NULL)
	{

	}

	HRESULT LowDX11Logic::CreateDevice()
	{
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG


		// D3D11 Device 생성
		hr = D3D11CreateDevice(
			NULL,															// [in, optional]	IDXGIAdapter				*pAdapter
			D3D_DRIVER_TYPE_HARDWARE,										// D3D_Driver_Type				DriverType
			NULL,															// HMODULE						Software
			createDeviceFlags,												// UINT						Flags
			NULL,															// [in, optional]	const D3D_FEATURE_LEVEL		*pFeatureLevels
			NULL,															// UINT						FeatureLevels
			D3D11_SDK_VERSION,												// UINT						SDKVersion
			&(_DXStorage->_device),											// [out, optional]	ID3D11Device				**ppDevice
			NULL,															// [out, optional]	D3D_FEATUER_LEVEL			*pFeatureLevel
			&(_DXStorage->_deviceContext)									// [out, optional]	ID3D11DeviceContext			**ppImmediateContext
		);

		return hr;
	}

	HRESULT LowDX11Logic::CreateSwapChain(int screenWidth, int screenHeight)
	{
		// Swap Chain Description 정의
		_DXStorage->_swapChainDesc.BufferDesc.Width = screenWidth;
		_DXStorage->_swapChainDesc.BufferDesc.Height = screenHeight;
		_DXStorage->_swapChainDesc.BufferDesc.RefreshRate.Numerator = 120;
		_DXStorage->_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		_DXStorage->_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_DXStorage->_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_DXStorage->_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		_DXStorage->_swapChainDesc.SampleDesc.Count = 1;
		_DXStorage->_swapChainDesc.SampleDesc.Quality = 0;
		_DXStorage->_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_DXStorage->_swapChainDesc.BufferCount = 2;
		_DXStorage->_swapChainDesc.OutputWindow = _DXStorage->_hWnd;
		_DXStorage->_swapChainDesc.Windowed = true;
		_DXStorage->_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		_DXStorage->_swapChainDesc.Flags = 0;

		// DXGI Factory 생성
		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&_DXStorage->_factory));

		if (hr != S_OK)
		{
			return hr;
		}

		// 스왑체인 생성
		hr = _DXStorage->_factory->CreateSwapChain(_DXStorage->_device, &(_DXStorage->_swapChainDesc), &(_DXStorage->_swapChain));

		return hr;
	}

	HRESULT LowDX11Logic::CreateMainRenderTarget()
	{
		hr = _DXStorage->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&(_DXStorage->_backBuffer));

		if (hr != S_OK)
		{
			return hr;
		}

		hr = _DXStorage->_device->CreateRenderTargetView(_DXStorage->_backBuffer, nullptr, &(_DXStorage->_mainRTV));

		return hr;
	}

	HRESULT LowDX11Logic::CreateDepthStencilViewAndState()
	{
		/// Depth Stencil Buffer
		// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
		_DXStorage->_backBuffer->GetDesc(&_DXStorage->_depthStencilBufferDesc);

		// Depth-Stencil Buffer를 위한 Texture Resource Description 구조체 정의
		// 백버퍼의 속성을 가져온 후, Format과 BindFlags만 바꾸어 쓴다.
		_DXStorage->_depthStencilBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		_DXStorage->_depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// Depth-Stencil Buffer 생성
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_depthStencilBufferDesc), NULL, &(_DXStorage->_depthStencilBuffer));

		if (hr != S_OK)
			return hr;


		/// Depth Stencil View
		//// Depth-Stencil Description 구조체 정의
		ZeroMemory(&(_DXStorage->_depthStencilDesc), sizeof(D3D11_DEPTH_STENCIL_DESC));
		_DXStorage->_depthStencilDesc.DepthEnable = true;
		_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Depth-Stencil State 생성
		// (depth-stencil state는 OM 스테이지에 depth-stencil 테스트를 수행하는 방법을 전달한다)
		_DXStorage->_device->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &(_DXStorage->_depthStencilState));

		//// Depth-Stencil View 생성
		//// (Resource로 View를 생성해야 파이프라인에 바인드할 수 있다)

		_DXStorage->_depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		_DXStorage->_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		_DXStorage->_depthStencilViewDesc.Flags = 0;

		hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_depthStencilView));

		if (hr != S_OK)
			return hr;


		/// Depth Buffer 시각화를 위한 SRV 생성
		_DXStorage->_shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		_DXStorage->_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MipLevels = -1;


		hr = _DXStorage->_device->CreateShaderResourceView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_shaderResourceViewDesc), &(_DXStorage->_depthStencilSRV));

		return hr;
	}

	HRESULT LowDX11Logic::CreateRasterizerStates()
	{
		// Solid
		ZeroMemory(&(_DXStorage->_solidDesc), sizeof(D3D11_RASTERIZER_DESC));
		_DXStorage->_solidDesc.FillMode = D3D11_FILL_SOLID;
		_DXStorage->_solidDesc.CullMode = D3D11_CULL_BACK;
		_DXStorage->_solidDesc.FrontCounterClockwise = false;
		_DXStorage->_solidDesc.DepthClipEnable = true;

		hr = _DXStorage->_device->CreateRasterizerState(&(_DXStorage->_solidDesc), &(_DXStorage->_solidState));

		if (hr != S_OK)
			return hr;

		// Wireframe
		ZeroMemory(&(_DXStorage->_wireframeDesc), sizeof(D3D11_RASTERIZER_DESC));
		_DXStorage->_wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		_DXStorage->_wireframeDesc.CullMode = D3D11_CULL_BACK;
		_DXStorage->_wireframeDesc.FrontCounterClockwise = false;
		_DXStorage->_wireframeDesc.DepthClipEnable = true;

		hr = _DXStorage->_device->CreateRasterizerState(&(_DXStorage->_wireframeDesc), &(_DXStorage->_wireframeState));

		return hr;
	}

	void LowDX11Logic::SetRasterizerrStates(ID3D11RasterizerState* rasterizerState)
	{
		_DXStorage->_deviceContext->RSSetState(rasterizerState);

		return;
	}

	void LowDX11Logic::CreateAndSetViewports()
	{
		// Viewport 구조체 생성
		CD3D11_VIEWPORT viewport(
			0.0f,
			0.0f,
			static_cast<float>(_DXStorage->_screenWidth),
			static_cast<float>(_DXStorage->_screenHeight)
		);

		// Viewport 지정
		_DXStorage->_deviceContext->RSSetViewports(1, &viewport);
	}

	void LowDX11Logic::PrepareRenderTargets()
	{
		// Set Depth Stencil State
		_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

		// Clear Main RTV and DSV
		_DXStorage->_deviceContext->ClearRenderTargetView(_DXStorage->_mainRTV, _DXStorage->_backgroundColor);
		_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void LowDX11Logic::BindRenderTargets()
	{

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
	}

	void LowDX11Logic::UnbindRenderTargets()
	{
		// TODO: null RTV 바인딩
	}

	void LowDX11Logic::Draw()
	{
		
	}

	void LowDX11Logic::Present()
	{
		_DXStorage->_swapChain->Present(0, 0);
	}

	HRESULT LowDX11Logic::ResizeSwapChainBuffers(int screenWidth, int screenHeight)
	{

		hr = _DXStorage->_swapChain->ResizeBuffers(2, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		if (hr != S_OK)
		{
			return hr;
		}
	}

	LowDX11Logic* LowDX11Logic::GetInstance()
	{
		static LowDX11Logic* tInstance = new LowDX11Logic();

		return tInstance;
	}

}