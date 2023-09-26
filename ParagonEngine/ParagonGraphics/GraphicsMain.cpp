#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"
#include "GraphicsResourceManager.h"

#include "../ParagonCore/TimeManager.h"
#include "../ParagonCore/CoreMain.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/APIMain.h"

#include "ParagonRenderer.h"
#include "Sprite.h"
#include "Font.h"

#include <windows.h>
#include <numbers>
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

namespace Pg::Graphics
{
	GraphicsMain::GraphicsMain(Pg::Core::CoreMain* core)
		: hr(NULL), _coreMain(core),
		_DXStorage(nullptr), _DXLogic(nullptr),
		_renderer(nullptr)
	{
		_DXStorage = LowDX11Storage::GetInstance();
		_DXLogic = LowDX11Logic::GetInstance();

		_renderer = std::make_unique<ParagonRenderer>();
		_tempObj = new Pg::Core::GameObject("Test");

		auto& timeSystem = singleton<Pg::Core::Time::TimeManager>();
		_timeManager = &timeSystem;

		auto& api = singleton<Pg::API::APIMain>();
		_api = &api;
		_api->Initialize();
	}

	GraphicsMain::~GraphicsMain()
	{
		delete _tempObj;
	}

	float time = 0.0f;
	Pg::Graphics::Sprite* sprite;
	Pg::Graphics::Sprite* sprite2;
	Pg::Graphics::Font* font;
	std::wstring text;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		// 초기화 관련
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


		// 테스트용 큐브
		_box = new TestCube(_DXStorage);
		_box->Initialize();

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		VertexShader* vertexShader = new VertexShader(_DXStorage, L"../x64/debug/VertexShader.cso", vertexDesc);
		PixelShader* pixelShader = new PixelShader(_DXStorage, L"../x64/debug/PixelShader.cso");
		
		vertexShader->AddConstantBuffer(&(_box->_cbData));

		_box->SetVertexShader(vertexShader);
		_box->SetPixelShader(pixelShader);
		
		// 카메라
		_camera = new TempCamera();
		_camera->SetPosition(float3(0.0f, 0.0f, -3.0f));
		_camera->SetLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);


		// 타임 매니저
		_timeManager->Initialize();

		// 2d 스프라이트
		sprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/cats.dds");
		sprite->SetPosition(0.0f, 0.0f);

		sprite2 = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/rabbits.dds");
		sprite2->SetPosition(0.0f, 200.0f);

		// 폰트
		font = new Font();
		font->SetPosition(10.0f, 410.0f);
		font->SetText(L"");
	}


	void GraphicsMain::Update(const Pg::Core::Scene* const scene, Pg::Core::CameraData cameraData)
	{
		_timeManager->TimeMeasure();
		float dt = _timeManager->GetDeltaTime();

		time += (10.0f * dt);

		// 디버그 정보 출력
		text = L"";
		text.append(L"DeltaTime: " + std::to_wstring(dt) + L"\n");
		text.append(L"Time: " + std::to_wstring(time) + L"\n");
		text.append(L"FPS: " + std::to_wstring(_timeManager->GetFrameRate()) + L"\n");
		text.append(L"Look Vector: (" + std::to_wstring(_camera->GetLook().x) + L", " + std::to_wstring(_camera->GetLook().y) + L", " + std::to_wstring(_camera->GetLook().z) + L")") ;
		font->SetText(text);

		//cbData.viewMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._viewMatrix);
		//cbData.projectionMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._projMatrix);
		//cbData.viewProjMatrix = DirectX::XMMatrixMultiply(cbData.viewMatrix, cbData.projectionMatrix);


		/// Input 관련
		///
		auto& tInput = singleton<Pg::API::Input::PgInput>();
		_inputManager = &tInput;

		using namespace Pg::API::Input;

		if (_inputManager->GetKey(MoveFront))
		{
			_camera->Walk(20.f * dt);
		}
		if (_inputManager->GetKey(MoveBack))
		{
			_camera->Walk(-20.f * dt);
		}
		if (_inputManager->GetKey(MoveLeft))
		{
			_camera->Strafe(-20.f * dt);
		}
		if (_inputManager->GetKey(MoveRight))
		{
			_camera->Strafe(20.f * dt);
		}
		if (_inputManager->GetKey(MoveUp))
		{
			_camera->WorldUpDown(20.f * dt);
		}
		if (_inputManager->GetKey(MoveDown))
		{
			_camera->WorldUpDown(-20.f * dt);
		}
		if (_inputManager->GetKey(MouseRight) && _inputManager->IsMouseMoving())
		{
			_camera->RotateY(3.0f * _inputManager->GetMouseDX());
			_camera->Pitch(3.0f * _inputManager->GetMouseDY());
		}

		_camera->UpdateViewMatrix();

		/// 상수 버퍼 채우기
		///
		// TODO: PgMath로 교체
		using namespace DirectX;
		//using namespace Pg::Math;
		// 
		// 월드 행렬
		float4x4 worldMatrix = XMMATRIX(XMMatrixIdentity());

		worldMatrix *= XMMatrixRotationX(time);
		worldMatrix *= XMMatrixRotationY(time);
		worldMatrix *= XMMatrixRotationZ(time);

		worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

		_box->_cbData.worldMatrix = worldMatrix;

		// 카메라 행렬
		_box->_cbData.viewMatrix = _camera->View();
		_box->_cbData.projectionMatrix = _camera->Proj();
		_box->_cbData.viewProjMatrix = _camera->ViewProj();

		_box->Update(dt);
	}

	void GraphicsMain::BeginRender()
	{
		_renderer->BeginRender();

	}

	void GraphicsMain::Render()
	{
		// test용 큐브 그리기
		_box->Draw();
		
		// test 스프라이트 그리기
		sprite->Draw();
		sprite2->Draw();

		// test 폰트 그리기
		font->Draw();

		_renderer->Render(_tempObj);
	}

	void GraphicsMain::EndRender()
	{
		_renderer->EndRender();
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

		_camera->SetLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
	}

	ID3D11Device* GraphicsMain::GetDevice()
	{
		return _DXStorage->_device;
	}

	ID3D11DeviceContext* GraphicsMain::GetDeviceContext()
	{
		return _DXStorage->_deviceContext;
	}

	Pg::Graphics::Manager::GraphicsResourceManager* GraphicsMain::GetGraphicsResourceManager()
	{
		if (this->_graphicsResourceManager == nullptr)
		{
			this->_graphicsResourceManager = Pg::Graphics::Manager::GraphicsResourceManager::Instance();
		}
		return _graphicsResourceManager;
	}
	
	

}