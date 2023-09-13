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

		hr = _DXLogic->CreateDevice();
		hr = _DXLogic->CreateSwapChain();
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		hr = _DXLogic->CreateRenderStates();
		hr = _DXLogic->SetRenderStates();
		hr = _DXLogic->CreateAndSetViewports();
	}

	void GraphicsMain::Update()
	{

	}

	void GraphicsMain::BeginRender()
	{
		// TODO: Clear RenderTargetView
		_DXLogic->ClearRenderTargetView();

		// TODO: Clear DepthStencilView
		_DXLogic->ClearDepthStencilView();

		// TODO: Bind Render Target
		_DXLogic->BindRenderTargets();
	}

	void GraphicsMain::Render()
	{

	}

	void GraphicsMain::EndRender()
	{

	}

	void GraphicsMain::Finalize()
	{
		delete _DXLogic;
		delete _DXStorage;
	}
}