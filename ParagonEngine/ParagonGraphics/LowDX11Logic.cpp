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

		// D3D11 Device 생성
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

		/// BufferDesc : 생성하고자 하는 후면 버퍼들의 속성
	  // 해상도 폭을 나타내는 가로, 세로 크기
		swapChainDesc.BufferDesc.Width = 0;               // 0으로 지정하면 런타임이 출력창에서 너비를 가져와 그 너비값을 스왑체인에 할당함
		swapChainDesc.BufferDesc.Height = 0;               // 0으로 지정하면 런타임이 출력창에서 너비를 가져와 그 너비값을 스왑체인에 할당함
		// RefreshRate : 새로고침 빈도르르 헤르츠 단위로 설정
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;            // 분자
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;            // 분모, 정수를 표현하는 경우 분모는 1이어야 함
		// Format : 디스플레이 포맷을 설정
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // 각 원소는 [0,1] 구간으로 사상되는 8비트 부호없는 성분 네 개로 이루어짐
		// ScanlineOrdering : 스캔라인 그리기 모드를 설명하는 열거형 멤버
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		// Scaling : 스케일링 모드를 설명하는 열거형 멤버
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// SampleDesc : 다중 표본화를 위해 추출할 표본 개수와 품질 수준 -> 현재 사용하지 않도록 1,0으로 지정
		// 삼중버퍼링을 사용하기 위해서는 다중표본화를 사용해선안됨
		//if (_enable4xMsaa)
		//{
		//   // Count : 픽셀 당 멀티 셈플 수
		//   swapChainDesc.SampleDesc.Count = 4;
		//   // Quality : 이미지 품질 수준 -> CheckMultisampleQualityLevels가 반환하는 레벨보다 작아야 함
		//   swapChainDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
		//}
		//else

		{
			// 안티엘리어싱이 없는 기본 셈플러 모드 
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}

		// BufferUsage : 버퍼의 용도를 서술, 백 버퍼의 표면 사용량과 cpu 액세스 옵션을 설명하는 열거형 멤버 -> 후면 버퍼에 렌더링을 할 것
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// BufferCount : 교환 사슬에서 사용할 후면 버퍼의 갯수 -> 일반적으로 이중버퍼링일 때 하나, 삼중버퍼링일 때 두 개
		swapChainDesc.BufferCount = 2;

		// OutputWindow : 렌더링 결과를 표시할 창의 핸들 -> 창 모드로 스왑 체인을 생성하고 사용자가 IDXGISwapChain::SetFullscreenState를 통해 스왑 체인을 전체 화면으로 변경할 수 있도록 허용하는 것이 좋음
		swapChainDesc.OutputWindow = (HWND)(_DXStorage->_hWnd);

		// Windowed : 창 모드를 원하면 true, 전체화면 모드를 원하면 false
		swapChainDesc.Windowed = true;

		// SwapEffect : 표면을 표시한 후 presentation 버퍼의 내용을 처리하는 옵션을 설명하는 열거형 멤버로 DXGI_SWAP_EFFECT_DISCARD를 지정하면 디스플레이 구동기가 가장 효율적인 제시방법을 선택
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// Flags : 스왑 체인 동작에 대한 옵션을 설명하는 열거형 멤버 -> DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH를 지정하면 전체화면 모드로 전환될 때 현재의 후면 버퍼 설정에 가장 잘 부합하는 디스플레이가 자동으로 선택됨
		swapChainDesc.Flags = 0;

		// IDXGIDevice : 이미지 데이터를 생성하는 DXGI 객체에 대한 파생 클래스를 구현
		IDXGIDevice* p_dxgiDevice = 0;
		// queryinterface의 첫번째 매개변수 : COM 객체에서 내가 요청한 IID를 넘겨줌
		// queryinterface의 두번째 매개변수 : COM 객체가 가지고 있는 인터페이스를 이 변수에 담아줌
		HR(_DXStorage->_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&p_dxgiDevice));

		// IDXGIAdapter : 디스플레이 서브시스템 (하나 이상의 GPU, DAC 및 비디오 메모리 포함)을 나타냄
		IDXGIAdapter* p_dxgiAdapter = 0;
		HR(p_dxgiDevice->GetAdapter(&p_dxgiAdapter));                           // msdn에서는 이 방법 사용
		//p_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&p_dxgiAdapter);   // 책에서는 이 방법 사용

		// IDXGIFactory : 전체 화면 전환을 처리하는 DXGI 객체를 생성하는 매서드를 구현
		IDXGIFactory* p_dxgiFactory = 0;
		HR(p_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_dxgiFactory));

		// CreateSwapChain (ID3D11Device를 가리키는 포인터, 교환사슬 서술 구조체를 가리키는 포인터, 생성된 교환사슬 인터페이스를 돌려줄 변수)
		DXGI_SWAP_CHAIN_DESC tempSwapChainDesc = swapChainDesc;
		HR(p_dxgiFactory->CreateSwapChain(_DXStorage->_device, &tempSwapChainDesc, &(_DXStorage->_swapChain)));

		// 획득한 COM 인터페이스들을 다 사용했으므로 해제
		p_dxgiFactory->Release();
		p_dxgiAdapter->Release();
		p_dxgiDevice->Release();

		return S_OK;


		/////여기서부터
		//// Swap Chain Description 정의
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
		//// DXGI Factory 생성
		//hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&_DXStorage->_factory));
		//
		//if (hr != S_OK)
		//{
		//	return hr;
		//}
		//
		//// 스왑체인 생성
		//hr = _DXStorage->_factory->CreateSwapChain(_DXStorage->_device, &(_DXStorage->_swapChainDesc), &(_DXStorage->_swapChain));
		//
		//return hr;
	}

	HRESULT LowDX11Logic::CreateMainRenderTarget()
	{
		HR(_DXStorage->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&(_DXStorage->_backBuffer)));
		HR(_DXStorage->_device->CreateRenderTargetView(_DXStorage->_backBuffer, nullptr, &(_DXStorage->_mainRTV)));

		// TODO: 메인렌더타겟 SRV 생성 및 쿼드로 출력하기
		//hr = _DXStorage->_device->CreateShaderResourceView(_DXStorage->_backBuffer, &_DXStorage->_shaderResourceViewDesc, &_DXStorage->_mainRTSRV);
		
		return S_OK;
	}

	HRESULT LowDX11Logic::CreateDepthStencilViewAndState()
	{
		/// Depth Stencil Buffer
		// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
		_DXStorage->_backBuffer->GetDesc(&_DXStorage->_bufferDesc);

		// Depth-Stencil Buffer를 위한 Texture Resource Description 구조체 정의
		// 백버퍼의 속성을 가져온 후, Format과 BindFlags만 바꾸어 쓴다.
		_DXStorage->_bufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		_DXStorage->_bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// Depth-Stencil Buffer 생성
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_bufferDesc), NULL, &(_DXStorage->_depthStencilBuffer));
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_bufferDesc), NULL, &(_DXStorage->_tempDepthStencilBuffer));

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

		_DXStorage->_depthStencilDesc.DepthEnable = true;
		_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		_DXStorage->_device->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &(_DXStorage->_2ndPassDepthStencilState));

		//// Depth-Stencil View 생성
		//// (Resource로 View를 생성해야 파이프라인에 바인드할 수 있다)

		_DXStorage->_depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		_DXStorage->_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		_DXStorage->_depthStencilViewDesc.Flags = 0;

		hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_depthStencilView));
		//hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_secondPassDepthStencilView));

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
			
			//Full Screen 그 자체, Do not touch.
			
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

			//얘도 한치의 오차가 있으면 안됨.

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