#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"

#include "../ParagonCore/TimeManager.h"
#include "../ParagonCore/CoreMain.h"
#include "../ParagonAPI/PgInput.h"

#include "ParagonRenderer.h"
#include "Sprite.h"
#include "Font.h"

#include <windows.h>
#include <numbers>

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

		// TODO: Storage는 static으로 만들어서 인자로 넘길 필요가 없도록 하자
	}

	GraphicsMain::~GraphicsMain()
	{
		delete _tempObj;
	}

	float time = 0.0f;
	Pg::Graphics::Sprite* sprite;
	Pg::Graphics::Sprite* sprite2;
	Pg::Graphics::Font* font;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		OutputDebugString(L"GraphicsGraphics!!!");

		/// 초기화 관련
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


		/// 쉐이더 셋팅 관련
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		_DXStorage->_testVertexShader = new VertexShader(_DXStorage, L"../x64/debug/VertexShader.cso", vertexDesc);
		_DXStorage->_testPixelShader = new PixelShader(_DXStorage, L"../x64/debug/PixelShader.cso");

		_DXStorage->_testVertexShader->Bind();
		_DXStorage->_testPixelShader->Bind();

		// TODO: Shader가 TestBox 안에 있어야 하나?
		// test용 큐	브
		_box = new TestCube(_DXStorage);
		_DXStorage->_testVertexShader->AddConstantBuffer(&(_box->_cbData));

		// 카메라 설정
		_camera = new TempCamera();
		_camera->SetPosition(float3(0.0f, 0.0f, -3.0f));
		_camera->SetLens(0.25f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);

		Pg::Core::Time::TimeManager::Instance()->Initialize();

		sprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/cats.dds");
		sprite->SetPosition(100.0f, 100.0f);

		sprite2 = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/rabbits.dds");
		sprite2->SetPosition(800.0f,600.0f);

		font = new Font();
		font->SetText(L"test text..");
		font->SetPosition(50.0f, 50.0f);
	}


	void GraphicsMain::Update(const Pg::Core::Scene* const scene, Pg::Core::CameraData cameraData)
	{
		Pg::Core::Time::TimeManager::Instance()->TimeMeasure();
		float dt = Pg::Core::Time::TimeManager::Instance()->GetDeltaTime();

		time += (1.0f * dt);

		//cbData.viewMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._viewMatrix);
		//cbData.projectionMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._projMatrix);
		//cbData.viewProjMatrix = DirectX::XMMatrixMultiply(cbData.viewMatrix, cbData.projectionMatrix);

		// TODO: PgMath로 교체
		using namespace DirectX;
		//using namespace Pg::Math;


		/// 상수 버퍼 채우기
		// 월드 행렬
		float4x4 worldMatrix = XMMATRIX(XMMatrixIdentity());

		worldMatrix *= XMMatrixRotationX(time);
		worldMatrix *= XMMatrixRotationY(time);
		worldMatrix *= XMMatrixRotationZ(time);

		worldMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);

		_box->_cbData.worldMatrix = worldMatrix;

		// 카메라 행렬
		//_camera->Walk(-1.0f * dt);
		_camera->UpdateViewMatrix();

		_box->_cbData.viewMatrix = _camera->View();
		_box->_cbData.projectionMatrix = _camera->Proj();
		_box->_cbData.viewProjMatrix = _camera->ViewProj();

		// 상수버퍼 업데이트
		for (auto& e : _DXStorage->_testVertexShader->_constantBuffers)
		{
			e->Update();
		}

	
		/// Input 관련
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