#pragma once

#include "DX11Headers.h"

/// <summary>
/// DX11 관련 로우 레벨 리소스를 저장한다
/// 
/// 2023. 09. 12.
/// </summary>
namespace Pg::Graphics
{
	class LowDX11Storage
	{
	public:
		LowDX11Storage();

	public:
		// 윈도우 핸들
		HWND _hWnd;

		// 디바이스, 컨텍스트
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		// 스왑체인 관련
		IDXGIFactory2* _factory;
		DXGI_SWAP_CHAIN_DESC1 _swapChainDesc;
		IDXGISwapChain1* _swapChain;
		ID3D11Texture2D* _backBuffer;

		// 메인 렌더 타겟
		ID3D11RenderTargetView* _mainRTV;
		
	};

}

