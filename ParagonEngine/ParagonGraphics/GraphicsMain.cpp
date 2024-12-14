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
#include "VisualEffectRenderer.h"
#include "Sprite.h"
#include "Font.h"

#include "Grid.h"
#include "Axis.h"
#include "RenderCubemap.h"


//<���� Graphics Resource�� ���>
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
		//API ��� �뵵�� ������ ������ GraphicsApiExporter�� ����.
		auto& tApiExporter = singleton<Pg::Graphics::GraphicsApiExporter>();
		_graphicsApiExporter = &tApiExporter;
		_graphicsApiExporter->Initialize(this);

		// �ʱ�ȭ ����
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

		//Default Input Layout ����.
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
		//Projection ����� ä���.
		this->_camData = cameraData;
		
		//DeltaTime �Ѱ��ֱ�.
		_renderer->SetDeltaTime(deltaTime);
	}

	void GraphicsMain::BeginRender()
	{
		_renderer->BeginRender();
	}

	void GraphicsMain::Render(Pg::Data::Scene* scene)
	{
		_renderer->Render(_camData);

		//DebugRender ���, �ϴ��� ����Ʈ�� �ѵξ���.
		_renderer->DebugRender(_camData);

		_renderer->PostProcessingRender(_camData);
		_renderer->UiRender(_camData);
		_renderer->FadeInOutRender();
	}

	//Pg::Data::GameObject* GraphicsMain::GetPickedGameObjectWithRatios(float widthRatio, float heightRatio)
	//{
	//	assert(0.0f <= widthRatio && widthRatio <= 1.0f);
	//	assert(0.0f <= heightRatio && heightRatio <= 1.0f);
	//
	//	static bool tDB = false;
	//	if (!tDB)
	//	{
	//		PG_WARN("���� widthRatio�� heightRatio�� �ݿ��ް� ���� �ʴ´�. ���߿� �����Ͷ� ������ �� �߰��� ����ؾ�!");
	//		tDB = true;
	//	}
	//
	//	if (_internalPickingMode)
	//	{
	//		///������ ��ŷ �ڵ� �ּ�ó��. �����Ͱ� ���ӿ�����Ʈ ������ �� �� ��! �̰� ������ ���ܳ���. 
	//		//if (_input->GetKeyDown(API::Input::MouseLeft))
	//		//{
	//		//	//Input�� 0-1�� ����ȭ�� Ratio�� ���� ��ȯ�Ѵ�.
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
	//	//Picking Mode�� �ƴϹǷ�, �ܼ� nullptr�� �����Ѵ�.
	//	return nullptr;
	//}

	void GraphicsMain::FinalRender()
	{
		//Main Render Target���� ������.
		_renderer->FinalRender(_camData);
		//�߰� Debugging�� ���� ���.
		_renderer->DebugQuadsOverlayRender();
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

		//�̰͸����� Projection Matrix�� �����ȴ�.
		_DXStorage->_screenWidth = screenWidth;
		_DXStorage->_screenHeight = screenHeight;

		// ���� �ڿ��� �Ҵ� ����
		ReleaseCOM(_DXStorage->_mainRTV);
		ReleaseCOM(_DXStorage->_depthStencilView);
		ReleaseCOM(_DXStorage->_depthStencilSRV);
		ReleaseCOM(_DXStorage->_depthStencilBuffer);
		ReleaseCOM(_DXStorage->_backBuffer);

		// �ٲ� ������� ���Ҵ�
		hr = _DXLogic->ResizeSwapChainBuffers(screenWidth, screenHeight);
		hr = _DXLogic->CreateMainRenderTarget();
		hr = _DXLogic->CreateDepthStencilViewAndState();
		_DXLogic->CreateAndSetViewports();
		// TODO: ���� ������ ���� �� ���Ҵ� ������ ���� ��

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
		//�� �Լ��� ȣ��Ǵ� ���� SceneSystem�̳� GameEngine ���ַ� �ϰ� �ϰ� �ʹ�.
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
		////������ File Path ( == AssetManager���� �̹� �ε��� ��ΰ� �־�� �۵��ϹǷ�, �ϵ��ڵ�����.)
		//
		//tFilePath = "../Resources/3DModels/Banana.fbx";
		//
		////MultiMaterial Mesh �׽���.
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
		//������ Editor Mode Enum ���.
		_prevRecordedEditMode = editorMode;
	}

	Pg::Graphics::VisualEffectRenderer* GraphicsMain::GetVisualEffectRenderer()
	{
		return _renderer->GetVisualEffectRenderer();
	}

	Pg::Data::CameraData* GraphicsMain::GetCamData()
	{
		return _camData;
	}

}