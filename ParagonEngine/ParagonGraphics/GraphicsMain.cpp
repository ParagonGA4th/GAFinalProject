#include "GraphicsMain.h"
#include <windows.h>
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	GraphicsMain::GraphicsMain()
		: hr(NULL),
		_DXStorage(nullptr), _DXLogic(nullptr)
	{
		_DXStorage = new LowDX11Storage();
		_DXLogic = new LowDX11Logic(_DXStorage);
	}

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		_DXStorage->_hWnd = hWnd;

		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;

		hr = _DXLogic->CreateDevice();
		hr = _DXLogic->CreateSwapChain(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		hr = _DXLogic->CreateRasterizerStates();
		
		_DXLogic->SetRasterizerrStates(_DXStorage->_solidState);
		_DXLogic->CreateAndSetViewports();
		
		// ½¦ÀÌ´õ ¼ÂÆÃ
		_DXLogic->SetShaders();

		_DXLogic->SetupCube();

	}

	void GraphicsMain::Update()
	{

	}

	void GraphicsMain::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
	}

	void GraphicsMain::Render()
	{
		_DXLogic->BindRenderTargets();
		_DXLogic->Draw();
	}

	void GraphicsMain::EndRender()
	{
		_DXLogic->Present();
	}

	void GraphicsMain::Finalize()
	{
		delete _DXLogic;
		delete _DXStorage;
	}
}