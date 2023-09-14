#pragma once

#include "DX11Headers.h"
#include "ConstantBuffer.h"

#include <vector>

/// <summary>
/// DX11 관련 로우 레벨 리소스를 저장한다
/// 
/// 2023. 09. 12. 김정식
/// </summary>
/// 
namespace Pg::Graphics
{
	struct CB_DATA
	{
		float degree;
		matrix viewMatrix;
		matrix projectionMatrix;
		matrix viewProjMatrix;
	};

	class LowDX11Storage
	{
	public:
		LowDX11Storage();

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
		D3D11_TEXTURE2D_DESC _depthStencilBufferDesc;
		D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC _shaderResourceViewDesc;

		// 스왑체인 관련
		IDXGIFactory2* _factory;
		IDXGISwapChain* _swapChain;
		ID3D11Texture2D* _backBuffer;

		// 메인 렌더 타겟
		ID3D11RenderTargetView* _mainRTV;

		// Depth-Stencil Buffer 관련
		ID3D11Texture2D* _depthStencilBuffer;
		ID3D11DepthStencilState* _depthStencilState;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11ShaderResourceView* _depthStencilSRV;

		// Rasterizer State
		D3D11_RASTERIZER_DESC _solidDesc;
		D3D11_RASTERIZER_DESC _wireframeDesc;
		ID3D11RasterizerState* _solidState;
		ID3D11RasterizerState* _wireframeState;

		const FLOAT _backgroundColor[4] = { 0.8f, 0.5f, 0.5f, 0.0f };

		ID3D11InputLayout* _inputLayout;


		// Shader 관련
		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader* _pixelShader;
		ID3DBlob* _vertexShaderByteCode;
		ID3DBlob* _pixelShaderByteCode;

		// 상수 버퍼 관련
		D3D11_BUFFER_DESC _ConstantBufferDesc;
		std::vector<ConstantBuffer<CB_DATA>*> _constantBuffers;
	};

}

