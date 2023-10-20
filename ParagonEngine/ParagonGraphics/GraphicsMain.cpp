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

	const float cameraSpeed = 10.0f;
	Pg::Graphics::Sprite* tempEditorCamSprite;
	Pg::Graphics::Sprite* tempGameCamSprite;

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
		_DXLogic->SetRasterizerStates(_DXStorage->_solidState);

		hr = _DXLogic->CreateSamplerStates();

		_DXLogic->CreateAndSetViewports();

		//Default Input Layout 세팅.
		LayoutDefine::Initialize();

		// Renderer 초기화
		_renderer->Initialize();

		// Camera
		_camera = new TempCamera(float3(0.0f, 3.0f, -10.0f));
		_camera->SetLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);


		//현재로서는 명시적으로 Editor를 위해 사진으로 SRV를 만들어 넘겨주지만, 나중에는 바뀌어야 한다! (실제 렌더되는 카메라 화면으로)
		//실제 Sprite로서 활용 X, DDS만 금방 만들기 위해서!
		tempEditorCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/EditorCamDummy.dds");
		tempEditorCamSprite->SetPosition(100.0f, 200.0f);

		tempGameCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/GameCamDummy.dds");
		tempGameCamSprite->SetPosition(400.0f, 200.0f);
	}


	void GraphicsMain::Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData, float deltaTime)
	{
		//당장 CameraData가 반영되는 것이 아님.
		//_timeManager->TimeMeasure();
		//float dt = _timeManager->GetDeltaTime();

		/// Input 관련
		///
		using namespace Pg::API::Input;

		if (_input->GetKey(MoveFront))
		{
			_camera->Walk(1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MoveBack))
		{
			_camera->Walk(-1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MoveLeft))
		{
			_camera->Strafe(-1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MoveRight))
		{
			_camera->Strafe(1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MoveUp))
		{
			_camera->WorldUpDown(1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MoveDown))
		{
			_camera->WorldUpDown(-1.0f * cameraSpeed * deltaTime);
		}
		if (_input->GetKey(MouseRight) && _input->IsMouseMoving())
		{
			_camera->RotateY(3.0f * _input->GetMouseDX());
			_camera->Pitch(3.0f * _input->GetMouseDY());
		}

		_camera->UpdateViewMatrix();


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

		// 렌더
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

	ID3D11ShaderResourceView* GraphicsMain::GetEditorCameraViewSRV()
	{
		_editorCameraSRV = tempEditorCamSprite->GetSRV();
		return _editorCameraSRV;
	}

	ID3D11ShaderResourceView* GraphicsMain::GetGameCameraViewSRV()
	{
		_gameCameraSRV = tempGameCamSprite->GetSRV();
		return _gameCameraSRV;
	}
}