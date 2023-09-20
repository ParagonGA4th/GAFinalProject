#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"

#include "../ParagonCore/TimeManager.h"

#include "../ParagonAPI/PgInput.h"

#include <windows.h>
#include <numbers>

namespace Pg::Graphics
{
	GraphicsMain::GraphicsMain()
		: hr(NULL),
		_DXStorage(nullptr), _DXLogic(nullptr)
	{
		_DXStorage = new LowDX11Storage();
		_DXLogic = new LowDX11Logic(_DXStorage);
	}

	float time = 0.0f;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		_DXStorage->_hWnd = hWnd;

		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;


		/// 초기화 관련
		hr = _DXLogic->CreateDevice();
		
		hr = _DXLogic->CreateSwapChain(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();

		hr = _DXLogic->CreateDepthStencilViewAndState();
		
		hr = _DXLogic->CreateRasterizerStates();
		_DXLogic->SetRasterizerrStates(_DXStorage->_solidState);
		
		_DXLogic->CreateAndSetViewports();


		/// 쉐이더 셋팅 관련
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		_DXStorage->_vertexShader = new VertexShader(_DXStorage, L"../x64/debug/VertexShader.cso", vertexDesc);
		_DXStorage->_pixelShader = new PixelShader(_DXStorage, L"../x64/debug/PixelShader.cso");


		_DXStorage->_vertexShader->Bind();
		_DXStorage->_pixelShader->Bind();

		// test용 큐	브 셋팅
		_box = new TestCube(_DXStorage);

		_DXStorage->_vertexShader->AddConstantBuffer(&(_box->_cbData));
		
			// 카메라 설정
		_camera = new TempCamera();
		_camera->SetPosition(float3(0.0f, 0.0f, -3.0f));
		_camera->SetLens(0.25f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);

		Pg::Core::Time::TimeManager::Instance()->Initialize();
	}


	void GraphicsMain::Update(const Pg::Core::Scene* const scene, Pg::Core::CameraData cameraData)
	{
		Pg::Core::Time::TimeManager::Instance()->TimeMeasure();
		float dt = Pg::Core::Time::TimeManager::Instance()->GetDeltaTime();

		time += (5.0f * dt);

		//cbData.viewMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._viewMatrix);
		//cbData.projectionMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._projMatrix);
		//cbData.viewProjMatrix = DirectX::XMMatrixMultiply(cbData.viewMatrix, cbData.projectionMatrix);

		//_box->Update(time);
		// TODO: ConstantBuffer가 Shader 안으로 들어가야 함.
		// TODO: Shader가 TestBox 안에 있어야 함.
		using namespace DirectX;
		//using namespace Pg::Math;

		float4x4 worldMatrix = XMMATRIX(XMMatrixIdentity());

		worldMatrix *= XMMatrixRotationX(time);
		worldMatrix *= XMMatrixRotationY(time);
		worldMatrix *= XMMatrixRotationZ(time);

		worldMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);

		_box->_cbData.worldMatrix = worldMatrix;

		_box->_cbData.viewMatrix = _camera->View();
		_box->_cbData.projectionMatrix = _camera->Proj();
		_box->_cbData.viewProjMatrix = _camera->ViewProj();

		for (auto& e : _DXStorage->_vertexShader->_constantBuffers)
		{
			e->Update();
		}

		//_DXStorage->_vertexShader->_constantBuffers->Update(&(_box->_cbData));

		_camera->Walk(-1.0f * dt);
		_camera->UpdateViewMatrix();

		//using namespace Pg::API::Input;
		//
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
	}

	void GraphicsMain::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
	}

	void GraphicsMain::Render()
	{
		_DXLogic->BindRenderTargets();
		
		// test용 큐브 그리기
		_box->Draw();
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
		ReleaseCOM(_DXStorage->_depthStencilSRV);
		ReleaseCOM(_DXStorage->_depthStencilBuffer);
		ReleaseCOM(_DXStorage->_backBuffer)

		// 바뀐 사이즈로 재할당
		hr = _DXLogic->ResizeSwapChainBuffers(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		_DXLogic->CreateAndSetViewports();

		//_camera->SetLens(0.25f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
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