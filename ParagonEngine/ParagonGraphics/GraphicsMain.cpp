#include "GraphicsMain.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "ConstantBuffer.h"
#include "MathHelper.h"
#include "GraphicsApiExporter.h"
#include "GraphicsResourceManager.h"
#include "LayoutDefine.h"

#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonUtil/InputSystem.h"

#include "ParagonRenderer.h"
#include "GeometryGenerator.h"
#include "AssimpBufferParser.h"
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
#include <cmath>
#include <limits>
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
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

		auto& tInput = singleton<Pg::Util::Input::InputSystem>();
		_input = &tInput;
	}

	GraphicsMain::~GraphicsMain()
	{
		
	}

	//
	Pg::Graphics::Sprite* tempGameCamSprite;

	void GraphicsMain::Initialize(HWND hWnd, int screenWidth, int screenHeight, const std::string& resourceListPath)
	{
		//API 사용 용도로 본인의 포인터 GraphicsApiExporter로 전달.
		auto& tApiExporter = singleton<Pg::Graphics::GraphicsApiExporter>();
		_graphicsApiExporter = &tApiExporter;
		_graphicsApiExporter->Initialize(this);

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
		GeometryGenerator::Initialize();
		Helper::AssimpBufferParser::Initialize();

		_renderer->Initialize(&_prevRecordedEditMode, resourceListPath);

		tempGameCamSprite = new Sprite(_DXStorage->_deviceContext, L"../Resources/Textures/DummyData/GameCamDummy.dds");
	}

	void GraphicsMain::GraphicsConnectDefaultResources()
	{
		_renderer->ConnectDefaultResources();
	}

	void GraphicsMain::Update(const Pg::Data::Scene* const scene, float deltaTime, Pg::Data::CameraData* cameraData)
	{
		//Projection 행렬을 채운다.
		this->_camData = cameraData;
		
		//DeltaTime 넘겨주기.
		_renderer->SetDeltaTime(deltaTime);
	}

	void GraphicsMain::BeginRender()
	{
		_renderer->BeginRender();
	}

	void GraphicsMain::Render(Pg::Data::Scene* scene)
	{
		_renderer->Render(_camData);

		//DebugRender 기능, 일단은 디폴트로 켜두었음.
		_renderer->DebugRender(_camData);

		_renderer->UiRender(_camData);

		_renderer->PostProcessingRender(_camData);
	}

	//Pg::Data::GameObject* GraphicsMain::GetPickedGameObjectWithRatios(float widthRatio, float heightRatio)
	//{
	//	assert(0.0f <= widthRatio && widthRatio <= 1.0f);
	//	assert(0.0f <= heightRatio && heightRatio <= 1.0f);
	//
	//	static bool tDB = false;
	//	if (!tDB)
	//	{
	//		PG_WARN("현재 widthRatio와 heightRatio는 반영받고 있지 않는다. 나중에 에디터랑 연동할 떄 추가해 사용해야!");
	//		tDB = true;
	//	}
	//
	//	if (_internalPickingMode)
	//	{
	//		///눈물의 피킹 코드 주석처리. 에디터가 게임오브젝트 선택한 거 줄 것! 이건 다음에 남겨놓자. 
	//		//if (_input->GetKeyDown(API::Input::MouseLeft))
	//		//{
	//		//	//Input이 0-1로 정규화된 Ratio로 값을 반환한다.
	//		//	widthRatio = _input->GetMouseX();
	//		//	heightRatio = _input->GetMouseY();
	//		//
	//		//	int selectedWidth = round(static_cast<float>(_DXStorage->_screenWidth) * widthRatio);
	//		//	int selectedHeight = round(static_cast<float>(_DXStorage->_screenHeight) * heightRatio);
	//		//
	//		//	//std::string tVal = "X : ";
	//		//	//tVal += std::to_string(selectedWidth);
	//		//	//tVal += " / Y : ";
	//		//	//tVal += std::to_string(selectedHeight);
	//		//	//PG_INFO(tVal.c_str());
	//		//	
	//		//	return _renderer->GetPickedID_SetOutlineMode(selectedWidth, selectedHeight);
	//		//}
	//		return nullptr;
	//	}
	//
	//	//Picking Mode가 아니므로, 단순 nullptr를 리턴한다.
	//	return nullptr;
	//}

	void GraphicsMain::FinalRender()
	{
		//Main Render Target으로 보내기.
		_renderer->FinalRender(_camData);
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
		ReleaseCOM(_DXStorage->_backBuffer);

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
		//_editorCameraSRV = tempEditorCamSprite->GetSRV();
		//return _editorCameraSRV;
		return _renderer->GetFinalQuadSRV();
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

	//void GraphicsMain::SyncLoadGraphicsResources()
	//{
	//	
	//}

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

	void GraphicsMain::SetRayCastDebugRenderData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec)
	{
		_renderer->PassRayCastGeometryData(rayCastColVec);
	}

	void GraphicsMain::SetBox2dDebugRenderData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec)
	{
		_renderer->PassBox2dGeometryData(box2dColVec);
	}

	void GraphicsMain::SetNavMeshDebugRenderData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec)
	{
		_renderer->PassNavMeshGeometryData(navMeshVec);
	}

	void GraphicsMain::SetNavCylinderDebugRenderData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec)
	{
		_renderer->PassNavCylinderGeometryData(navCylinderVec);
	}

	void GraphicsMain::SetPickingEnableMode(bool val)
	{
		_internalPickingMode = val;
	}

	void GraphicsMain::SetOutlinedObject(Pg::Data::GameObject* obj)
	{
		_renderer->SetOutlinedGameObject(obj);
	}

	void GraphicsMain::AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_renderer->AddRenderObject_Runtime(objVecP);
	}

	void GraphicsMain::ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_renderer->ModifyRenderObject_Runtime(objVecP);
	}

	void GraphicsMain::DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_renderer->DeleteRenderObject_Runtime(objVecP);
	}

	void GraphicsMain::HandleRenderObjectsRuntime()
	{
		_renderer->HandleRenderObjectsRuntime();
	}

	void GraphicsMain::SetEditorMode(Pg::Data::Enums::eEditorMode editorMode)
	{
		//기존의 Editor Mode Enum 기록.
		_prevRecordedEditMode = editorMode;
	}
}