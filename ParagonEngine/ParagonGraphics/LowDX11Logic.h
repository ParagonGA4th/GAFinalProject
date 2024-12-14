#pragma once

#include "DX11Headers.h"
#include <string>

/// <summary>
/// DX11�� ���õ� ������ ĸ��ȭ�Ѵ�.
/// 
/// 2023. 09. 12. ������
/// </summary>
/// 

namespace Pg::Graphics
{
	class LowDX11Storage;

	class LowDX11Logic
	{
	public:
		static LowDX11Logic* GetInstance();

	private:
		LowDX11Logic();
		LowDX11Logic(const LowDX11Logic& rhs) = delete;
		LowDX11Logic(LowDX11Logic&& rhs) = delete;
		LowDX11Logic& operator=(const LowDX11Logic& rhs) = delete;
		LowDX11Logic& operator=(LowDX11Logic&& rhs) = delete;

	public:
		HRESULT CreateDevice();
		HRESULT CreateSwapChain(int screenWidth, int screenHeight);
		HRESULT CreateMainRenderTarget();
		HRESULT CreateDepthStencilViewAndState();
		HRESULT CreateRasterizerStates();
		HRESULT CreateSamplerStates();
		HRESULT CreateBlendState();
		void SetRasterizerStates(ID3D11RasterizerState* rasterizerState);
		void CreateAndSetViewports();

		void Present();

	public:
		HRESULT ResizeSwapChainBuffers(int screenWidth, int screenHeight);

	private:
		
	public:
		LowDX11Storage* _DXStorage;

	private:
		HRESULT hr;
	};

}

