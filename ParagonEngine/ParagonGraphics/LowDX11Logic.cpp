#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
//#include "RenderShader.h"
#include "DX11Headers.h"
#include "../ParagonData/GameConstantData.h"

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

		// D3D11 Device ����
		HR(D3D11CreateDevice(
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
		));

		D3D_FEATURE_LEVEL tCheckFeatureLevel = _DXStorage->_device->GetFeatureLevel();
		assert(tCheckFeatureLevel == D3D_FEATURE_LEVEL_11_0 || tCheckFeatureLevel == D3D_FEATURE_LEVEL_11_1);
		return S_OK;
	}

	HRESULT LowDX11Logic::CreateSwapChain(int screenWidth, int screenHeight)
	{
		DXGI_SWAP_CHAIN_DESC& swapChainDesc = _DXStorage->_swapChainDesc;

		/// BufferDesc : �����ϰ��� �ϴ� �ĸ� ���۵��� �Ӽ�
	  // �ػ� ���� ��Ÿ���� ����, ���� ũ��
		swapChainDesc.BufferDesc.Width = 0;               // 0���� �����ϸ� ��Ÿ���� ���â���� �ʺ� ������ �� �ʺ��� ����ü�ο� �Ҵ���
		swapChainDesc.BufferDesc.Height = 0;               // 0���� �����ϸ� ��Ÿ���� ���â���� �ʺ� ������ �� �ʺ��� ����ü�ο� �Ҵ���
		// RefreshRate : ���ΰ�ħ �󵵸��� �츣�� ������ ����
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;            // ����
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;            // �и�, ������ ǥ���ϴ� ��� �и�� 1�̾�� ��
		// Format : ���÷��� ������ ����
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // �� ���Ҵ� [0,1] �������� ���Ǵ� 8��Ʈ ��ȣ���� ���� �� ���� �̷����
		// ScanlineOrdering : ��ĵ���� �׸��� ��带 �����ϴ� ������ ���
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		// Scaling : �����ϸ� ��带 �����ϴ� ������ ���
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// SampleDesc : ���� ǥ��ȭ�� ���� ������ ǥ�� ������ ǰ�� ���� -> ���� ������� �ʵ��� 1,0���� ����
		// ���߹��۸��� ����ϱ� ���ؼ��� ����ǥ��ȭ�� ����ؼ��ȵ�
		//if (_enable4xMsaa)
		//{
		//   // Count : �ȼ� �� ��Ƽ ���� ��
		//   swapChainDesc.SampleDesc.Count = 4;
		//   // Quality : �̹��� ǰ�� ���� -> CheckMultisampleQualityLevels�� ��ȯ�ϴ� �������� �۾ƾ� ��
		//   swapChainDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
		//}
		//else

		{
			// ��Ƽ��������� ���� �⺻ ���÷� ��� 
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}

		// BufferUsage : ������ �뵵�� ����, �� ������ ǥ�� ��뷮�� cpu �׼��� �ɼ��� �����ϴ� ������ ��� -> �ĸ� ���ۿ� �������� �� ��
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// BufferCount : ��ȯ �罽���� ����� �ĸ� ������ ���� -> �Ϲ������� ���߹��۸��� �� �ϳ�, ���߹��۸��� �� �� ��
		swapChainDesc.BufferCount = 2;

		// OutputWindow : ������ ����� ǥ���� â�� �ڵ� -> â ���� ���� ü���� �����ϰ� ����ڰ� IDXGISwapChain::SetFullscreenState�� ���� ���� ü���� ��ü ȭ������ ������ �� �ֵ��� ����ϴ� ���� ����
		swapChainDesc.OutputWindow = (HWND)(_DXStorage->_hWnd);

		// Windowed : â ��带 ���ϸ� true, ��üȭ�� ��带 ���ϸ� false
		swapChainDesc.Windowed = true;

		// SwapEffect : ǥ���� ǥ���� �� presentation ������ ������ ó���ϴ� �ɼ��� �����ϴ� ������ ����� DXGI_SWAP_EFFECT_DISCARD�� �����ϸ� ���÷��� �����Ⱑ ���� ȿ������ ���ù���� ����
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// Flags : ���� ü�� ���ۿ� ���� �ɼ��� �����ϴ� ������ ��� -> DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH�� �����ϸ� ��üȭ�� ���� ��ȯ�� �� ������ �ĸ� ���� ������ ���� �� �����ϴ� ���÷��̰� �ڵ����� ���õ�
		swapChainDesc.Flags = 0;

		// IDXGIDevice : �̹��� �����͸� �����ϴ� DXGI ��ü�� ���� �Ļ� Ŭ������ ����
		IDXGIDevice* p_dxgiDevice = 0;
		// queryinterface�� ù��° �Ű����� : COM ��ü���� ���� ��û�� IID�� �Ѱ���
		// queryinterface�� �ι�° �Ű����� : COM ��ü�� ������ �ִ� �������̽��� �� ������ �����
		HR(_DXStorage->_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&p_dxgiDevice));

		// IDXGIAdapter : ���÷��� ����ý��� (�ϳ� �̻��� GPU, DAC �� ���� �޸� ����)�� ��Ÿ��
		IDXGIAdapter* p_dxgiAdapter = 0;
		HR(p_dxgiDevice->GetAdapter(&p_dxgiAdapter));                           // msdn������ �� ��� ���
		//p_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&p_dxgiAdapter);   // å������ �� ��� ���

		// IDXGIFactory : ��ü ȭ�� ��ȯ�� ó���ϴ� DXGI ��ü�� �����ϴ� �ż��带 ����
		IDXGIFactory* p_dxgiFactory = 0;
		HR(p_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_dxgiFactory));

		// CreateSwapChain (ID3D11Device�� ����Ű�� ������, ��ȯ�罽 ���� ����ü�� ����Ű�� ������, ������ ��ȯ�罽 �������̽��� ������ ����)
		DXGI_SWAP_CHAIN_DESC tempSwapChainDesc = swapChainDesc;
		HR(p_dxgiFactory->CreateSwapChain(_DXStorage->_device, &tempSwapChainDesc, &(_DXStorage->_swapChain)));

		// ȹ���� COM �������̽����� �� ��������Ƿ� ����
		p_dxgiFactory->Release();
		p_dxgiAdapter->Release();
		p_dxgiDevice->Release();

		return S_OK;


		/////���⼭����
		//// Swap Chain Description ����
		//_DXStorage->_swapChainDesc.BufferDesc.Width = screenWidth;
		//_DXStorage->_swapChainDesc.BufferDesc.Height = screenHeight;
		//_DXStorage->_swapChainDesc.BufferDesc.RefreshRate.Numerator = 120;
		//_DXStorage->_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		//_DXStorage->_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//_DXStorage->_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//_DXStorage->_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		//_DXStorage->_swapChainDesc.SampleDesc.Count = 1;
		//_DXStorage->_swapChainDesc.SampleDesc.Quality = 0;
		//_DXStorage->_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//_DXStorage->_swapChainDesc.BufferCount = 2;
		//_DXStorage->_swapChainDesc.OutputWindow = _DXStorage->_hWnd;
		//_DXStorage->_swapChainDesc.Windowed = true;
		//_DXStorage->_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		//_DXStorage->_swapChainDesc.Flags = 0;
		//
		//// DXGI Factory ����
		//hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&_DXStorage->_factory));
		//
		//if (hr != S_OK)
		//{
		//	return hr;
		//}
		//
		//// ����ü�� ����
		//hr = _DXStorage->_factory->CreateSwapChain(_DXStorage->_device, &(_DXStorage->_swapChainDesc), &(_DXStorage->_swapChain));
		//
		//return hr;
	}

	HRESULT LowDX11Logic::CreateMainRenderTarget()
	{
		HR(_DXStorage->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&(_DXStorage->_backBuffer)));
		HR(_DXStorage->_device->CreateRenderTargetView(_DXStorage->_backBuffer, nullptr, &(_DXStorage->_mainRTV)));

		// TODO: ���η���Ÿ�� SRV ���� �� ����� ����ϱ�
		//hr = _DXStorage->_device->CreateShaderResourceView(_DXStorage->_backBuffer, &_DXStorage->_shaderResourceViewDesc, &_DXStorage->_mainRTSRV);
		
		return S_OK;
	}

	HRESULT LowDX11Logic::CreateDepthStencilViewAndState()
	{
		/// Depth Stencil Buffer
		// DSB�� �����ϱ� ���� BackBuffer�� ������ ������
		_DXStorage->_backBuffer->GetDesc(&_DXStorage->_bufferDesc);

		// Depth-Stencil Buffer�� ���� Texture Resource Description ����ü ����
		// ������� �Ӽ��� ������ ��, Format�� BindFlags�� �ٲپ� ����.
		_DXStorage->_bufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		_DXStorage->_bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// Depth-Stencil Buffer ����
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_bufferDesc), NULL, &(_DXStorage->_depthStencilBuffer));
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_bufferDesc), NULL, &(_DXStorage->_tempDepthStencilBuffer));

		if (hr != S_OK)
			return hr;


		/// Depth Stencil View
		//// Depth-Stencil Description ����ü ����
		ZeroMemory(&(_DXStorage->_depthStencilDesc), sizeof(D3D11_DEPTH_STENCIL_DESC));
		_DXStorage->_depthStencilDesc.DepthEnable = true;
		_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Depth-Stencil State ����
		// (depth-stencil state�� OM ���������� depth-stencil �׽�Ʈ�� �����ϴ� ����� �����Ѵ�)
		_DXStorage->_device->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &(_DXStorage->_depthStencilState));

		_DXStorage->_depthStencilDesc.DepthEnable = true;
		_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		_DXStorage->_device->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &(_DXStorage->_2ndPassDepthStencilState));

		//// Depth-Stencil View ����
		//// (Resource�� View�� �����ؾ� ���������ο� ���ε��� �� �ִ�)

		_DXStorage->_depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		_DXStorage->_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		_DXStorage->_depthStencilViewDesc.Flags = 0;

		hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_depthStencilView));
		//hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_secondPassDepthStencilView));

		if (hr != S_OK)
			return hr;


		/// Depth Buffer �ð�ȭ�� ���� SRV ����
		_DXStorage->_shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		_DXStorage->_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MipLevels = -1;


		hr = _DXStorage->_device->CreateShaderResourceView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_shaderResourceViewDesc), &(_DXStorage->_depthStencilSRV));

		return hr;
	}

	HRESULT LowDX11Logic::CreateRasterizerStates()
	{
		// Solid (Backface)
		{
			ZeroMemory(&(_DXStorage->_solidDesc), sizeof(D3D11_RASTERIZER_DESC));
			_DXStorage->_solidDesc.FillMode = D3D11_FILL_SOLID;
			_DXStorage->_solidDesc.CullMode = D3D11_CULL_BACK;
			_DXStorage->_solidDesc.FrontCounterClockwise = false;
			_DXStorage->_solidDesc.DepthClipEnable = false;

			HR(_DXStorage->_device->CreateRasterizerState(&(_DXStorage->_solidDesc), &(_DXStorage->_solidState)));
		}
		

		// Solid (FrontFace)
		{
			ZeroMemory(&(_DXStorage->_solidFrontfaceCullingDesc), sizeof(D3D11_RASTERIZER_DESC));
			_DXStorage->_solidFrontfaceCullingDesc.FillMode = D3D11_FILL_SOLID;
			_DXStorage->_solidFrontfaceCullingDesc.CullMode = D3D11_CULL_FRONT;
			_DXStorage->_solidFrontfaceCullingDesc.FrontCounterClockwise = false;
			_DXStorage->_solidFrontfaceCullingDesc.DepthClipEnable = false;

			HR(_DXStorage->_device->CreateRasterizerState(
				&(_DXStorage->_solidFrontfaceCullingDesc), &(_DXStorage->_solidFrontfaceCullingState)));
		}

		// Solid (NOCULL)
		{
			ZeroMemory(&(_DXStorage->_solidNoCullingDesc), sizeof(D3D11_RASTERIZER_DESC));
			_DXStorage->_solidNoCullingDesc.FillMode = D3D11_FILL_SOLID;
			_DXStorage->_solidNoCullingDesc.CullMode = D3D11_CULL_NONE;
			_DXStorage->_solidNoCullingDesc.FrontCounterClockwise = false;
			_DXStorage->_solidNoCullingDesc.DepthClipEnable = false;

			HR(_DXStorage->_device->CreateRasterizerState(
				&(_DXStorage->_solidNoCullingDesc), &(_DXStorage->_solidNoCullingState)));
		}
		
		// Wireframe
		{
			ZeroMemory(&(_DXStorage->_wireframeDesc), sizeof(D3D11_RASTERIZER_DESC));
			_DXStorage->_wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
			_DXStorage->_wireframeDesc.CullMode = D3D11_CULL_BACK;
			_DXStorage->_wireframeDesc.FrontCounterClockwise = false;
			_DXStorage->_wireframeDesc.DepthClipEnable = true;

			HR(_DXStorage->_device->CreateRasterizerState(&(_DXStorage->_wireframeDesc), &(_DXStorage->_wireframeState)));
		}
		
		return S_OK;
	}

	void LowDX11Logic::SetRasterizerStates(ID3D11RasterizerState* rasterizerState)
	{
		_DXStorage->_deviceContext->RSSetState(rasterizerState);

		return;
	}

	void LowDX11Logic::CreateAndSetViewports()
	{
		// Viewport ����ü ���� -> Default.
		CD3D11_VIEWPORT defaultViewport(
			0.0f,
			0.0f,
			static_cast<float>(_DXStorage->_screenWidth),
			static_cast<float>(_DXStorage->_screenHeight)
		);
		_DXStorage->_defaultViewport = defaultViewport;

		CD3D11_VIEWPORT shadowMapViewport(
			0.f,
			0.f,
			static_cast<float>(Pg::Data::GameConstantData::SHADOW_MAP_LENGTH),
			static_cast<float>(Pg::Data::GameConstantData::SHADOW_MAP_LENGTH)
		);
		_DXStorage->_shadowMapViewport = shadowMapViewport;

		// Viewport ���� -> Default.
		_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
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

	HRESULT LowDX11Logic::CreateSamplerStates()
	{
		//fullScreenQuadSS
		{
			D3D11_SAMPLER_DESC tDesc;

			tDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			//tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.MaxAnisotropy = 16;
			tDesc.MipLODBias = 0.0f;
			//tDesc.MinLOD = 0.0f;
			//tDesc.MaxLOD = D3D11_FLOAT32_MAX;
			
			//Full Screen �� ��ü, Do not touch.
			
			HR(_DXStorage->_device->CreateSamplerState(&tDesc, &(_DXStorage->_fullScreenQuadSamplerState)));
		}

		//lightmapSS
		{
			D3D11_SAMPLER_DESC tDesc;

			tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.MaxAnisotropy = 1;
			tDesc.MipLODBias = 0.0f;

			//�굵 ��ġ�� ������ ������ �ȵ�.

			HR(_DXStorage->_device->CreateSamplerState(&tDesc, &(_DXStorage->_lightmapSamplerState)));
		}

		//DefaultTexturesSS
		{
			D3D11_SAMPLER_DESC tDesc;

			// = Trilinear.
			//tDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			tDesc.MaxAnisotropy = 16;
			tDesc.MipLODBias = 0.0f;

			tDesc.MinLOD = 0.0f;
			tDesc.MaxLOD = D3D11_FLOAT32_MAX;
			
			HR(_DXStorage->_device->CreateSamplerState(&tDesc, &(_DXStorage->_defaultSamplerState)));
		}

		//blurSS
		{
			D3D11_SAMPLER_DESC tDesc;

			tDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			tDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			tDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			tDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			tDesc.MipLODBias = 0.0f;
			tDesc.MaxAnisotropy = 1;

			tDesc.MinLOD = 0.0f;
			tDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HR(_DXStorage->_device->CreateSamplerState(&tDesc, &(_DXStorage->_lutSamplerState)));
		}


		return S_OK;
	}

	HRESULT LowDX11Logic::CreateBlendState()
	{
		D3D11_BLEND_DESC tDesc ;

		for (auto& e : tDesc.RenderTarget)
		{
			e.BlendEnable = true;
			e.SrcBlend = D3D11_BLEND_ONE;
			e.DestBlend = D3D11_BLEND_ZERO;
			e.BlendOp = D3D11_BLEND_OP_ADD;
			e.SrcBlendAlpha = D3D11_BLEND_ONE;
			e.DestBlendAlpha = D3D11_BLEND_ZERO;
			e.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			e.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		hr = _DXStorage->_device->CreateBlendState(&tDesc, &(_DXStorage->_blendState));

		return hr;
	}

}