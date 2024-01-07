#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"
#include "GraphicsResourceManager.h"
#include "LayoutDefine.h"

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
#include "RenderCubemap.h"


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

		hr = _DXLogic->CreateSamplerStates();

		hr = _DXLogic->CreateBlendState();

		_DXLogic->CreateAndSetViewports();

		//Default Input Layout 세팅.
		LayoutDefine::Initialize();

		_renderer->Initialize();

		//BasicRendersInitialize();
		tempEditorCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/EditorCamDummy.dds");
		tempEditorCamSprite->SetPosition(100.0f, 200.0f);

		tempGameCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/GameCamDummy.dds");
		tempGameCamSprite->SetPosition(400.0f, 200.0f);
	}


	void GraphicsMain::Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData)
	{
		//Projection 행렬을 채운다.
		FillCamDataProjection(cameraData);
		this->_camData = cameraData;

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
			_renderer->ParseSceneData(scene);
			_currentScene = scene;
		}
		assert(_currentScene != nullptr);

		_renderer->Render(_camData);
		//DebugRender 기능, 일단은 디폴트로 켜두었음.
		_renderer->DebugRender(_camData);
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

		//이것만으로 Projection Matrix는 조정된다.
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
		// TODO: 쿼드 사이즈 변경 후 재할당 로직도 들어가야 함

	}

	ID3D11Device* GraphicsMain::GetDevice()
	{
		return _DXStorage->_device;
	}

	ID3D11DeviceContext* GraphicsMain::GetDeviceContext()
	{
		return _DXStorage->_deviceContext;
	}

	void GraphicsMain::SyncComponentToGraphics(const Pg::Data::Scene* const scene)
	{
		//이 함수가 호출되는 것은 SceneSystem이나 GameEngine 주최로 하게 하고 싶다.
		_renderer->SyncComponentToGraphics(scene);
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

	void GraphicsMain::FillCamDataProjection(Pg::Data::CameraData* camData)
	{
		camData->_nearWindowHeight = 2.0f * camData->_nearZ * tanf(0.5f * camData->_fovY);
		camData->_farWindowHeight = 2.0f * camData->_farZ * tanf(0.5f * camData->_fovY);

		camData->_aspect = static_cast<float>(_DXStorage->_screenWidth) / static_cast<float>(_DXStorage->_screenHeight);
		camData->_projMatrix = Pg::Math::PGMatrixPerspectiveFovLH(camData->_fovY, camData->_aspect, camData->_nearZ, camData->_farZ);
	}

	void GraphicsMain::SyncLoadGraphicsResources()
	{
		//TempResourceMeshLoad();
	}

	void GraphicsMain::TempResourceMeshLoad()
	{
		//std::string tFilePath;
		////고정된 File Path ( == AssetManager에서 이미 로딩된 경로가 있어야 작동하므로, 하드코딩했음.)
		//
		//tFilePath = "../Resources/3DModels/Banana.fbx";
		//
		////MultiMaterial Mesh 테스팅.
		//_tempMultiMesh = new MultimaterialMesh(tFilePath);
	}

	void GraphicsMain::SetBoxDebugRenderData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec)
	{
		_renderer->PassBoxGeometryData(boxColVec);
	}

	void GraphicsMain::SetLineDebugRenderData(const std::vector<Pg::Data::LineInfo>& const lineColVec)
	{
		_renderer->PassLineGeometryData(lineColVec);
	}

	void GraphicsMain::SetCapsuleDebugRenderData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec)
	{
		_renderer->PassCapsuleGeometryData(capsuleColVec);
	}

	void GraphicsMain::SetSphereDebugRenderData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec)
	{
		_renderer->PassSphereGeometryData(sphereColVec);
	}

	void GraphicsMain::SetPlaneDebugRenderData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec)
	{
		_renderer->PassPlaneGeometryData(planeColVec);
	}

}