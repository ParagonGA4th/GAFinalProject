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
		// D3D11 Device 생성
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

		// hWnd 사용하여 스왑체인 생성
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
		/// Depth Stencil Buffer
		// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
		_DXStorage->_backBuffer->GetDesc(&_DXStorage->_depthStencilBufferDesc);

		// Depth-Stencil Buffer를 위한 Texture Resource Description 구조체 정의
		// 백버퍼의 속성을 가져온 후, Format과 BindFlags만 바꾸어 쓴다.
		_DXStorage->_depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		_DXStorage->_depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		// Depth-Stencil Buffer 생성
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_depthStencilBufferDesc), NULL, &(_DXStorage->_depthStencilBuffer));

		if (hr != S_OK)
			return hr;

		/// Depth Stencil View
		// Depth-Stencil Description 구조체 정의
		//ZeroMemory(&(_DXStorage->_depthStencilDesc), sizeof(D3D11_DEPTH_STENCIL_DESC));
		//_DXStorage->_depthStencilDesc.DepthEnable = true;
		//_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		//// Depth-Stencil State 생성
		//// (depth-stencil state는 OM 스테이지에 depth-stencil 테스트를 수행하는 방법을 전달한다)
		//m_pDevice->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &m_pDepthStencilState);

		//// Depth-Stencil View 생성
		//// (Resource로 View를 생성해야 파이프라인에 바인드할 수 있다)

		//hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);



		//DSB_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//DSB_d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//hr = m_pDevice->CreateTexture2D(&DSB_d, NULL, &m_pDepthStencilBuffer_2);

		//if (FAILED(hr))
		//{
		//	return false;
		//}



		//// Depth-Stencil 버퍼 시각화를 위해 Shader Resource View 생성
		//D3D11_SHADER_RESOURCE_VIEW_DESC SRV_d;
		//SRV_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//SRV_d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		//hr = m_pDevice->CreateShaderResourceView(m_pDepthStencilBuffer_2, nullptr, &DSBSRV);

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