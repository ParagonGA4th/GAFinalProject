#pragma once

#include "DX11Headers.h"
#include <string>

/// <summary>
/// DX11°ú °ü·ÃµÈ ·ÎÁ÷À» Ä¸½¶È­ÇÑ´Ù.
/// 
/// 2023. 09. 12. ±èÁ¤½Ä
/// </summary>
/// 

namespace Pg::Graphics
{
	class LowDX11Storage;
	class CB_DATA;

	class LowDX11Logic
	{
	public:
		LowDX11Logic(LowDX11Storage* DXStorage);

	public:
		HRESULT CreateDevice();
		HRESULT CreateSwapChain(int screenWidth, int screenHeight);
		HRESULT CreateMainRenderTarget();
		HRESULT CreateDepthStencilViewAndState();
		HRESULT CreateRasterizerStates();
		void SetRasterizerrStates(ID3D11RasterizerState* rasterizerState);
		void CreateAndSetViewports();

		void SetVertexShader(std::wstring CSOFilePath);
		void SetPixelShader(std::wstring CSOFilePath);

		void PrepareRenderTargets();
		void BindRenderTargets();
		void UnbindRenderTargets();

		void Draw();

		void Present();

	public:
		HRESULT ResizeSwapChainBuffers(int screenWidth, int screenHeight);

	private:
		
	public:
		LowDX11Storage* _DXStorage;

	private:
		HRESULT hr;


	public:
		void SetupCube();
		void DrawCube();
	};

}

