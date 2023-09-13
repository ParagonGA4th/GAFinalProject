#pragma once

#include "DX11Headers.h"

/// <summary>
/// DX11°ú °ü·ÃµÈ ·ÎÁ÷À» Ä¸½¶È­ÇÑ´Ù.
/// 
/// 2023. 09. 12. ±èÁ¤½Ä
/// </summary>
/// 

namespace Pg::Graphics
{
	class LowDX11Storage;

	class LowDX11Logic
	{
	public:
		LowDX11Logic(LowDX11Storage* DXStorage);

	public:
		HRESULT CreateDevice();
		HRESULT CreateSwapChain();
		HRESULT CreateMainRenderTarget();
		HRESULT CreateDepthStencilViewAndState();
		HRESULT CreateRenderStates();
		HRESULT SetRenderStates();
		HRESULT CreateAndSetViewports();

		void ClearRenderTargetView();
		void ClearDepthStencilView();
		void BindRenderTargets();
		void UnbindRenderTargets();

	public:
		LowDX11Storage* _DXStorage;

	private:
		HRESULT hr;
	};

}

