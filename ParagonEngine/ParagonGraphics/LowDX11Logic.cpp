#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	LowDX11Logic::LowDX11Logic(LowDX11Storage* DXStorage)
		:_DXStorage(DXStorage),
		hr(NULL)
	{

	}

	HRESULT LowDX11Logic::CreateDevice()
	{
		hr = D3D11CreateDevice(
			NULL,															// [in, optional]	IDXGIAdapter				*pAdapter
			D3D_DRIVER_TYPE_HARDWARE,										//					D3D_Driver_Type				DriverType
			NULL,															//					HMODULE						Software
			D3D11_CREATE_DEVICE_DEBUG,									//					UINT						Flags
			NULL,															// [in, optional]	const D3D_FEATURE_LEVEL		*pFeatureLevels
			NULL,															//					UINT						FeatureLevels
			D3D11_SDK_VERSION,												//					UINT						SDKVersion
			&(_DXStorage->_device),														// [out, optional]	ID3D11Device				**ppDevice
			NULL,															// [out, optional]	D3D_FEATUER_LEVEL			*pFeatureLevel
			&(_DXStorage->_deviceContext)												// [out, optional]	ID3D11DeviceContext			**ppImmediateContext
		);

		return hr;
	}

	HRESULT LowDX11Logic::CreateSwapChain()
	{
		// Swap Chain Description 정의
		_DXStorage->_swapChainDesc.Width = 0;
		_DXStorage->_swapChainDesc.Height = 0;
		_DXStorage->_swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_DXStorage->_swapChainDesc.Stereo = FALSE;
		_DXStorage->_swapChainDesc.SampleDesc.Count = 1;
		_DXStorage->_swapChainDesc.SampleDesc.Quality = 0;
		_DXStorage->_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_DXStorage->_swapChainDesc.BufferCount = 2;
		_DXStorage->_swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		_DXStorage->_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		_DXStorage->_swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		_DXStorage->_swapChainDesc.Flags = NULL;
		// DXGI Factory 생성

		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&_DXStorage->_factory));

		if (hr != S_OK)
		{
			return hr;
		}

		// Hwnd 사용하여 스왑체인 생성
		hr = _DXStorage->_factory->CreateSwapChainForHwnd(
			_DXStorage->_device,
			_DXStorage->_hWnd,
			&(_DXStorage->_swapChainDesc),
			NULL,
			NULL,
			&(_DXStorage->_swapChain)
		);

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
		return hr;
	}

	HRESULT LowDX11Logic::CreateRenderStates()
	{
		return hr;
	}

	HRESULT LowDX11Logic::SetRenderStates()
	{
		return hr;
	}

	HRESULT LowDX11Logic::CreateAndSetViewports()
	{
		return hr;
	}

	void LowDX11Logic::ClearRenderTargetView()
	{

	}

	void LowDX11Logic::ClearDepthStencilView()
	{

	}

	void LowDX11Logic::BindRenderTargets()
	{

	}

	void LowDX11Logic::UnbindRenderTargets()
	{

	}

}