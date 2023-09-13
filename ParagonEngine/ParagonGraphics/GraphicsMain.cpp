#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

#include "../ParagonCore/TimeManager.h"

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

	struct CB_DATA
	{
		float degree;
	};
	CB_DATA cb;
	float time = 0.0f;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		_DXStorage->_hWnd = hWnd;

		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;

		// 디바이스 생성 
		hr = _DXLogic->CreateDevice();
		
		// 스왑체인과 메인 렌더타겟 생성
		hr = _DXLogic->CreateSwapChain(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();

		// depth stencil view와 depth stencil state 생성
		hr = _DXLogic->CreateDepthStencilViewAndState();
		
		// Rasterizer State 설정
		hr = _DXLogic->CreateRasterizerStates();
		_DXLogic->SetRasterizerrStates(_DXStorage->_solidState);
		
		// 뷰포트 설정
		_DXLogic->CreateAndSetViewports();
		
		// 쉐이더 설정
		_DXLogic->SetVertexShader(L"../x64/debug/VertexShader.cso");
		_DXLogic->SetPixelShader(L"../x64/debug/PixelShader.cso");

		// 상수 버퍼


		_DXStorage->_ConstantBufferDesc.ByteWidth = 16;//sizeof(CB_DATA);
		_DXStorage->_ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_DXStorage->_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &cb;

		hr = _DXStorage->_device->CreateBuffer(&(_DXStorage->_ConstantBufferDesc), &data, &(_DXStorage->_ConstantBuffer));


		// test용 큐	브 셋팅
		_DXLogic->SetupCube();

	}


	void GraphicsMain::Update()
	{
		Pg::Core::Time::TimeManager::Instance()->Initialize();
		Pg::Core::Time::TimeManager::Instance()->TimeMeasure();
		time += Pg::Core::Time::TimeManager::Instance()->GetDeltaTime();

		cb.degree = time;
		
		_DXStorage->_deviceContext->UpdateSubresource(_DXStorage->_ConstantBuffer, 0, NULL, &cb, 0, 0);

		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &(_DXStorage->_ConstantBuffer));


		
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

		// 바뀐 사이즈로 다시 재할당
		hr = _DXLogic->ResizeSwapChainBuffers(screenHeight, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		_DXLogic->CreateAndSetViewports();
	}

}