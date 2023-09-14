#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"

#include "../ParagonCore/TimeManager.h"

#include "../ParagonAPI/PgInput.h"

#include <windows.h>

namespace Pg::Graphics
{
	GraphicsMain::GraphicsMain()
		: hr(NULL),
		_DXStorage(nullptr), _DXLogic(nullptr)
	{
		_DXStorage = new LowDX11Storage();
		_DXLogic = new LowDX11Logic(_DXStorage);
	}

	Pg::Graphics::CB_DATA cbData;
	float time = 0.0f;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		_DXStorage->_hWnd = hWnd;

		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;

		_camera = new TempCamera();

		hr = _DXLogic->CreateDevice();
		
		hr = _DXLogic->CreateSwapChain(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();

		hr = _DXLogic->CreateDepthStencilViewAndState();
		
		hr = _DXLogic->CreateRasterizerStates();
		_DXLogic->SetRasterizerrStates(_DXStorage->_solidState);
		
		_DXLogic->CreateAndSetViewports();
		
		_DXLogic->SetVertexShader(L"../x64/debug/VertexShader.cso");
		_DXLogic->SetPixelShader(L"../x64/debug/PixelShader.cso");

		_DXStorage->_constantBuffers.emplace_back(new ConstantBuffer(_DXStorage, &cbData));

		// test용 큐	브 셋팅
		_DXLogic->SetupCube();

	}


	void GraphicsMain::Update()
	{
		Pg::Core::Time::TimeManager::Instance()->Initialize();
		Pg::Core::Time::TimeManager::Instance()->TimeMeasure();
		float dt = Pg::Core::Time::TimeManager::Instance()->GetDeltaTime();

		cbData.degree += (1.0f * dt);
		
		for (auto& e : _DXStorage->_constantBuffers)
		{
			e->Update(&cbData);
		}


		//using namespace Pg::API::Input;

		//if (PgInput::GetKeyDown(MoveFront))
		//{
		//	_camera->Walk(10.f * dt);
		//}
		//if (PgInput::GetKeyDown(MoveBack))
		//{
		//	_camera->Walk(-10.f * dt);
		//}
		//if (PgInput::GetKeyDown(MoveLeft))
		//{
		//	_camera->Strafe(-10.f * dt);
		//}
		//if (PgInput::GetKeyDown(MoveRight))
		//{
		//	_camera->Strafe(10.f * dt);
		//}
		//if (PgInput::GetKeyDown(MoveUp))
		//{
		//	_camera->WorldUpDown(-10.f * dt);
		//}
		//if (PgInput::GetKeyDown(MoveDown))
		//{
		//	_camera->WorldUpDown(10.f * dt);
		//}

		_camera->UpdateViewMatrix();
		cbData.viewMatrix = _camera->View();
		cbData.projectionMatrix = _camera->Proj();
	}

	void GraphicsMain::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
	}

	void GraphicsMain::Render()
	{
		_DXLogic->BindRenderTargets();
		
		// test용 큐브 그리기
		_DXLogic->DrawCube();
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

	void GraphicsMain::OnWindowResized(int screenWidth, int screenHeight)
	{
		OutputDebugString(L"Window Resized!!!");

		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;

		// 기존 자원들 할당 해제
		ReleaseCOM(_DXStorage->_mainRTV);
		ReleaseCOM(_DXStorage->_depthStencilView);
		ReleaseCOM(_DXStorage->_depthStencilBuffer);
		ReleaseCOM(_DXStorage->_depthStencilSRV);

		// 바뀐 사이즈로 재할당
		hr = _DXLogic->ResizeSwapChainBuffers(screenHeight, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		_DXLogic->CreateAndSetViewports();
	}

	ID3D11Device* GraphicsMain::GetDevice()
	{
		return _DXStorage->_device;
	}

	ID3D11DeviceContext* GraphicsMain::GetDeviceContext()
	{
		return _DXStorage->_deviceContext;
	}

}