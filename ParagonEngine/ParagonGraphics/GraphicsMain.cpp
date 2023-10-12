#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"
#include "GraphicsResourceManager.h"
#include "LayoutDefine.h"

#include "../ParagonProcess/TimeManager.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "../ParagonAPI/PgInput.h"

#include "ParagonRenderer.h"
#include "Sprite.h"
#include "Font.h"

#include "Grid.h"
#include "Axis.h"
#include "Cubemap.h"

//<실제 Graphics Resource의 목록>
#include "RenderMaterial.h"
#include "RenderTexture2D.h"
#include "Asset3DModelData.h"
//</>

//DirectXMesh Testing.
#include <dxmesh/DirectXMesh.h>

#include <cstring>
#include <windows.h>
#include <numbers>
#include <cassert>
#include <limits>
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	
	GraphicsMain::GraphicsMain(Pg::Core::ProcessMain* core)
		: hr(NULL), _coreMain(core),
		_DXStorage(nullptr), _DXLogic(nullptr),
		_renderer(nullptr), _graphicsResourceManager(Manager::GraphicsResourceManager::Instance())
	{
		_DXStorage = LowDX11Storage::GetInstance();
		_DXLogic = LowDX11Logic::GetInstance();

		_renderer = std::make_unique<ParagonRenderer>();
		_tempObj = new Pg::Data::GameObject("Test");

		auto& tInput = singleton<Pg::API::Input::PgInput>();
		_input = &tInput;
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

	Grid* grid;
	Axis* axis;

	Cubemap* cubemap;

	const float cameraSpeed = 100.0f;

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

		//Default Input Layout 세팅.
		LayoutDefine::Initialize();

		// 테스트용 큐브
		_box = new TestCube();
		_box->Initialize();

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}//,
			//{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		VertexShader* BoxVertexShader = new VertexShader(_DXStorage, L"../Builds/x64/debug/VertexShader.cso", vertexDesc);
		PixelShader* BoxPixelShader = new PixelShader(_DXStorage, L"../Builds/x64/debug/PixelShader.cso");
		
		// TODO: 비직관적이다.
		BoxVertexShader->AssignConstantBuffer(&(_box->_cbData));

		_box->AssignVertexShader(BoxVertexShader);
		_box->AssignPixelShader(BoxPixelShader);
		
		// Grid
		grid = new Grid();
		grid->Initialize();

		// Axis
		axis = new Axis();
		axis->Initialize();

		// TODO: TestBox와 Grid, Axis 모두 같은 InputLayout을 사용하고 있다...
		VertexShader* helperVS = new VertexShader(_DXStorage, L"../Builds/x64/debug/VertexShader.cso", vertexDesc);
		helperVS->AssignConstantBuffer(&(grid->_cbData));

		grid->AssignVertexShader(helperVS);
		grid->AssignPixelShader(BoxPixelShader);

		axis->AssignVertexShader(helperVS);
		axis->AssignPixelShader(BoxPixelShader);
		

		// Cubemap
		D3D11_INPUT_ELEMENT_DESC CubemapvertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		//DXMesh Testing, 임시
		uint32_t tOffsets[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT];
		uint32_t tStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		ComputeInputLayout(CubemapvertexDesc, std::size(CubemapvertexDesc), tOffsets, tStrides);

		VertexShader* CubemapVS = new VertexShader(_DXStorage, L"../Builds/x64/debug/CubemapVS.cso", CubemapvertexDesc);
		PixelShader* CubemapPS = new PixelShader(_DXStorage, L"../Builds/x64/debug/CubemapPS.cso");

		cubemap = new Cubemap();
		cubemap->Initialize();

		CubemapVS->AssignConstantBuffer(&(cubemap->_cbData));
		cubemap->AssignVertexShader(CubemapVS);
		cubemap->AssignPixelShader(CubemapPS);


		// Camera
		_camera = new TempCamera(float3(0.0f, 3.0f, -10.0f));
		_camera->SetLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);

		// 2DSprite
		sprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/cats.dds");
		sprite->SetPosition(0.0f, 0.0f);

		sprite2 = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/rabbits.dds");
		sprite2->SetPosition(0.0f, 200.0f);

		// Font
		font = new Font();
		font->SetPosition(10.0f, 410.0f);
		font->SetText(L"");
	}


	PARAGON_GRAPHICS_DLL void GraphicsMain::Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData, float deltaTime)
	{
		//_timeManager->TimeMeasure();
		//float dt = _timeManager->GetDeltaTime();

		float dt = deltaTime;
		time += dt;

		// 디버그 정보 출력
		text = L"";
		text.append(L"DeltaTime: " + std::to_wstring(dt) + L"\n");
		text.append(L"Time: " + std::to_wstring(time) + L" sec" + L"\n");

		float tFrameRate = -1.0f;
		if (dt > std::numeric_limits<float>::epsilon())
		{
			tFrameRate = static_cast<double>(1) / dt;
		}

		text.append(L"FPS: " + std::to_wstring(tFrameRate) + L"\n");
		text.append(L"Look Vector: (" + std::to_wstring(_camera->GetLook().x) + L", " + std::to_wstring(_camera->GetLook().y) + L", " + std::to_wstring(_camera->GetLook().z) + L")") ;
		font->SetText(text);

		//cbData.viewMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._viewMatrix);
		//cbData.projectionMatrix = Pg::Graphics::MathHelper::PG2XM_MATRIX(cameraData._projMatrix);
		//cbData.viewProjMatrix = DirectX::XMMatrixMultiply(cbData.viewMatrix, cbData.projectionMatrix);


		/// Input 관련
		///
		using namespace Pg::API::Input;
		
		if (_input->GetKey(MoveFront))
		{
			_camera->Walk(1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MoveBack))
		{
			_camera->Walk(-1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MoveLeft))
		{
			_camera->Strafe(-1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MoveRight))
		{
			_camera->Strafe(1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MoveUp))
		{
			_camera->WorldUpDown(1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MoveDown))
		{
			_camera->WorldUpDown(-1.0f * cameraSpeed * dt);
		}
		if (_input->GetKey(MouseRight) && _input->IsMouseMoving())
		{
			_camera->RotateY(3.0f * _input->GetMouseDX());
			_camera->Pitch(3.0f * _input->GetMouseDY());
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
		//_box->_cbData.worldMatrix = XMMATRIX(XMMatrixIdentity());
		grid->_cbData.worldMatrix = XMMATRIX(XMMatrixIdentity());

		// 카메라 행렬
		_box->_cbData.viewMatrix = _camera->View();
		_box->_cbData.projectionMatrix = _camera->Proj();
		_box->_cbData.viewProjMatrix = _camera->ViewProj();
		_box->_cbData.eyePos = _camera->GetPosition();

		grid->_cbData.viewMatrix = _camera->View();
		grid->_cbData.projectionMatrix = _camera->Proj();
		grid->_cbData.viewProjMatrix = _camera->ViewProj();

		cubemap->_cbData.worldMatrix = XMMatrixTranslation(_camera->GetPosition().x, _camera->GetPosition().y, _camera->GetPosition().z);
		cubemap->_cbData.viewMatrix = _camera->View();
		cubemap->_cbData.projectionMatrix = _camera->Proj();
		cubemap->_cbData.viewProjMatrix = _camera->ViewProj();
		cubemap->_cbData.worldViewProjMatrix = _camera->ViewProj() * XMMATRIX(XMMatrixIdentity());

	}

	void GraphicsMain::BeginRender()
	{
		_renderer->BeginRender();
	}

	
	void GraphicsMain::Render(Pg::Data::Scene* scene)
	{
		//렌더하기 전에 Scene이 바뀌었는지 체크.
		if (scene != _currentScene)
		{
			//새로 Scene이 바뀌었을 경우 RenderObject 구성을 바꾼다.
			//나중에는 Load 로직이 별도로 들어가야.
			_renderer->OnNewSceneStart(scene);
			_currentScene = scene;
		}
		assert(_currentScene != nullptr);

		//하드코딩된 리소스들.
		cubemap->Draw();
		
		// test용 큐브 그리기
		_box->Draw();
		// Grid
		grid->Draw();
		// Axis
		axis->Draw();
		
		// test 스프라이트 그리기
		sprite->Draw();
		sprite2->Draw();

		// test 폰트 그리기
		font->Draw();

		// test용 큐브 그리기
		_box->Draw();

		// #ForwardTemp: 임시로 직접 TempCamera -> CameraData로 옮기는 중.
		Pg::Data::CameraData tCamData;
		tCamData._position = { _camera->GetPosition().x, _camera->GetPosition().y,_camera->GetPosition().z};
		tCamData._rotation = { 0.f,0.f, 0.f, 0.f };
		
		DirectX::XMFLOAT4X4 tViewFF;
		DirectX::XMStoreFloat4x4(&tViewFF, _camera->View());
		DirectX::XMFLOAT4X4 tProjFF;
		DirectX::XMStoreFloat4x4(&tProjFF, _camera->Proj());

		assert(sizeof(DirectX::XMFLOAT4X4) == sizeof(Pg::Math::PGFLOAT4X4));
		std::memcpy(&(tCamData._viewMatrix), &tViewFF, sizeof(Pg::Math::PGFLOAT4X4));
		std::memcpy(&(tCamData._projMatrix), &tProjFF, sizeof(Pg::Math::PGFLOAT4X4));

		_renderer->Render(tCamData);
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

	void GraphicsMain::SyncComponentToGraphics()
	{
		_renderer->SyncComponentToGraphics();
	}

	void GraphicsMain::LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{
		_graphicsResourceManager->LoadResource(filePath, define);
	}

	void GraphicsMain::UnloadResource(const std::string& filePath)
	{
		_graphicsResourceManager->UnloadResource(filePath);
	}

	std::map<std::string, Pg::Data::Enums::eAssetDefine>* GraphicsMain::SendAddedSecondaryResources()
	{
		return _graphicsResourceManager->GetSecondaryResources();
	}

	void GraphicsMain::ClearSecondaryResourcesList()
	{
		_graphicsResourceManager->ClearSecondaryResourcesList();
	}


}