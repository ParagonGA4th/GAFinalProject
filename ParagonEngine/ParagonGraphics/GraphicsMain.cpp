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


	void GraphicsMain::Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData, float deltaTime)
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

	void GraphicsMain::FillCamDataProjection(Pg::Data::CameraData* camData)
	{
		camData->_nearWindowHeight = 2.0f * camData->_nearZ * tanf(0.5f * camData->_fovY);
		camData->_farWindowHeight = 2.0f * camData->_farZ * tanf(0.5f * camData->_fovY);

		camData->_aspect = static_cast<float>(_DXStorage->_screenWidth) / static_cast<float>(_DXStorage->_screenHeight);
		camData->_projMatrix = Pg::Math::PGMatrixPerspectiveFovLH(camData->_fovY, camData->_aspect, camData->_nearZ, camData->_farZ);
	}

	//void GraphicsMain::BasicRendersInitialize()
	//{
	//	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//	{
	//		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}//,
	//		//{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//	};

	//	VertexShader* BoxVertexShader = new VertexShader(_DXStorage, L"../Builds/x64/debug/VertexShader.cso", vertexDesc);
	//	PixelShader* BoxPixelShader = new PixelShader(_DXStorage, L"../Builds/x64/debug/PixelShader.cso");

	//	// Grid
	//	grid = new Grid();
	//	grid->Initialize();

	//	// Axis
	//	axis = new Axis();
	//	axis->Initialize();

	//	// TODO: TestBox와 Grid, Axis 모두 같은 InputLayout을 사용하고 있다...
	//	VertexShader* helperVS = new VertexShader(_DXStorage, L"../Builds/x64/debug/VertexShader.cso", vertexDesc);
	//	helperVS->AssignConstantBuffer(&(grid->_cbData));

	//	grid->AssignVertexShader(helperVS);
	//	grid->AssignPixelShader(BoxPixelShader);

	//	axis->AssignVertexShader(helperVS);
	//	axis->AssignPixelShader(BoxPixelShader);

	//	// Cubemap
	//	D3D11_INPUT_ELEMENT_DESC CubemapvertexDesc[] =
	//	{
	//		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//	};

	//	//DXMesh Testing, 임시
	//	uint32_t tOffsets[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT];
	//	uint32_t tStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	//	ComputeInputLayout(CubemapvertexDesc, std::size(CubemapvertexDesc), tOffsets, tStrides);

	//	VertexShader* CubemapVS = new VertexShader(_DXStorage, L"../Builds/x64/debug/CubemapVS.cso", CubemapvertexDesc);
	//	PixelShader* CubemapPS = new PixelShader(_DXStorage, L"../Builds/x64/debug/CubemapPS.cso");

	//	cubemap = new Cubemap();
	//	cubemap->Initialize();

	//	CubemapVS->AssignConstantBuffer(&(cubemap->_cbData));
	//	cubemap->AssignVertexShader(CubemapVS);
	//	cubemap->AssignPixelShader(CubemapPS);


	//	// Camera
	//	//_camera = new TempCamera(float3(0.0f, 3.0f, -10.0f));
	//	//_camera->SetLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);

	//	// 2DSprite
	//	sprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/cats.dds");
	//	sprite->SetPosition(0.0f, 0.0f);

	//	sprite2 = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/rabbits.dds");
	//	sprite2->SetPosition(0.0f, 200.0f);

	//	// Font
	//	font = new Font();
	//	font->SetPosition(10.0f, 410.0f);
	//	font->SetText(L"");

	//	//현재로서는 명시적으로 Editor를 위해 사진으로 SRV를 만들어 넘겨주지만, 나중에는 바뀌어야 한다! (실제 렌더되는 카메라 화면으로)
	//	//실제 Sprite로서 활용 X, DDS만 금방 만들기 위해서!
	//	tempEditorCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/EditorCamDummy.dds");
	//	tempEditorCamSprite->SetPosition(100.0f, 200.0f);

	//	tempGameCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/GameCamDummy.dds");
	//	tempGameCamSprite->SetPosition(400.0f, 200.0f);
	//}

	void GraphicsMain::TempResourceMeshLoad()
	{
		std::string tFilePath;
		//고정된 File Path ( == AssetManager에서 이미 로딩된 경로가 있어야 작동하므로, 하드코딩했음.)
		//tFilePath = "../Resources/3DModels/TexturedMultiCubes/TMultiCube_test001.fbx";
		//tFilePath = "../Resources/3DModels/MultiMatMesh/LavaWoodCone.fbx";
		//tFilePath = "../Resources/3DModels/MultiMatMesh/TwoRoadWoodTorus.fbx";
		//tFilePath = "../Resources/3DModels/TexturedMultiCubes/TMultiCube_test002.fbx";
		//tFilePath = "../Resources/3DModels/MultiMatMesh/diffuseonly.fbx";
		//tFilePath = "../Resources/3DModels/TexturedMultiCubes/Floor_test003.fbx";
		//tFilePath = "../Resources/3DModels/TexturedMultiCubes/Floor_test003.fbx";
		//tFilePath = "../Resources/3DModels/TexturedMultiCubes/TexturedMultiCubeMultiMeshSeams.fbx";
		//tFilePath = "../Resources/3DModels/Banana.fbx";

		//MultiMaterial Mesh 테스팅.
		//_tempMultiMesh = new MultimaterialMesh(tFilePath);
		//_tempMultiMesh->Initialize();
	}

	void GraphicsMain::SyncLoadGraphicsResources()
	{
		TempResourceMeshLoad();
	}

}