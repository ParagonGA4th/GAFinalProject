#pragma once

#include "DX11Headers.h"
#include "ConstantBuffer.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include <vector>

/// <summary>
/// DX11 관련 로우 레벨 리소스를 저장한다
/// 
/// 2023. 09. 12. 김정식
/// </summary>
/// 
namespace Pg::Graphics
{
	class LowDX11Storage
	{
	public:
		static LowDX11Storage* GetInstance();

	private:
		LowDX11Storage();
		LowDX11Storage(const LowDX11Storage& rhs) = delete;
		LowDX11Storage(LowDX11Storage&& rhs) = delete;
		LowDX11Storage& operator=(const LowDX11Storage& rhs) = delete;
		LowDX11Storage& operator=(LowDX11Storage&& rhs) = delete;

	public:
		// 윈도우 핸들
		HWND _hWnd;

		// 스크린 크기
		int _screenWidth;
		int _screenHeight;

		// 디바이스, 컨텍스트
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		// Description 구조체들
		DXGI_SWAP_CHAIN_DESC _swapChainDesc;
		D3D11_TEXTURE2D_DESC _bufferDesc;
		D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC _shaderResourceViewDesc;
		D3D11_BUFFER_DESC _ConstantBufferDesc;

		// 스왑체인 관련
		IDXGIFactory2* _factory;
		IDXGISwapChain* _swapChain;
		ID3D11Texture2D* _backBuffer;

		// 메인 렌더 타겟
		ID3D11RenderTargetView* _mainRTV;
		ID3D11ShaderResourceView* _mainRTSRV;

		// Depth-Stencil Buffer 관련
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11DepthStencilState* _depthStencilState;
		ID3D11DepthStencilState* _2ndPassDepthStencilState;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11ShaderResourceView* _depthStencilSRV;
		ID3D11Texture2D* _tempDepthStencilBuffer;
		ID3D11DepthStencilView* _secondPassDepthStencilView;

		// Rasterizer State
		D3D11_RASTERIZER_DESC _solidDesc;
		D3D11_RASTERIZER_DESC _solidFrontfaceCullingDesc;
		D3D11_RASTERIZER_DESC _solidNoCullingDesc;
		D3D11_RASTERIZER_DESC _wireframeDesc;
		ID3D11RasterizerState* _solidState;
		ID3D11RasterizerState* _solidFrontfaceCullingState;
		ID3D11RasterizerState* _solidNoCullingState;
		ID3D11RasterizerState* _wireframeState;

		// Sampler State
		ID3D11SamplerState* _defaultSamplerState;
		ID3D11SamplerState* _lightmapSamplerState;
		ID3D11SamplerState* _fullScreenQuadSamplerState;
		ID3D11SamplerState* _lutSamplerState;

		ID3D11BlendState* _blendState;

		//const FLOAT _backgroundColor[4] = { 0.6f, 0.5f, 0.5f, 1.0f };
		const FLOAT _backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		CD3D11_VIEWPORT _defaultViewport;
		CD3D11_VIEWPORT _shadowMapViewport;

	};

}

