#include "GraphicsMain.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	GraphicsMain::GraphicsMain()
		:_DXStorage(), _DXLogic()
	{
		_DXStorage = new LowDX11Storage();
		_DXLogic = new LowDX11Logic(_DXStorage);

		_DXLogic->_DXStorage = _DXStorage;
	}

	void GraphicsMain::Initialize()
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		// TODO: Window Handle을 인자로 받아와야 함. 그러기 위해서 부모 클래스인 IEngine 인터페이스를 수정해야함.
		//_DXStorage->_hWnd = hWnd;
		_DXLogic->CreateDevice();
		//_DXLogic->CreateSwapChain();
		//_DXLogic->CreateMainRenderTarget();
		_DXLogic->CreateDepthStencilViewAndState();
		_DXLogic->CreateRenderStates();
		_DXLogic->SetRenderStates();
		_DXLogic->CreateAndSetViewports();
	}

	void GraphicsMain::Update()
	{
		// TODO: Clear RenderTargetView

		// TODO: Clear DepthStencilView

		// TODO: Bind Render Target
	}

	void GraphicsMain::Finalize()
	{

	}



}