#include "ProcessMain.h"
#include "../ParagonUtil/UtilMain.h"
#include "../ParagonAPI/APIMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "AssetManager.h"
#include "EditorAdapter.h"
#include "EngineGraphicsAdapter.h"

#include <string>
#include <windows.h>
#include <singleton-cpp/singleton.h>

#pragma once
#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

namespace Pg::Core
{
	ProcessMain::ProcessMain() : _engineGraphicsAdapter(nullptr), _editorAdapter(nullptr)
	{
		//���� ���� ���� ����.
		auto& tAssetManager = singleton<Manager::AssetManager>();
		_assetManager = &tAssetManager;

		_util = std::make_unique<Pg::Util::UtilMain>();
		_api = std::make_unique<Pg::API::APIMain>();

		_engineGraphicsAdapter = std::make_unique<EngineGraphicsAdapter>(this);
		_editorAdapter = std::make_unique<EditorAdapter>(this);
	}

	ProcessMain::~ProcessMain()
	{

	}

	long ProcessMain::Initialize(void* hwnd, int screenWidth, int screenHeight)
	{
		//���߿� �ϰ������� Editor�� ResourceList ������ �̷������ �Ѵ� (string)
		std::string tResourceListPath = "../ResourceList";

		//���� �ʱ�ȭ
		_util->Initialize(tResourceListPath, screenWidth, screenHeight, hwnd);
		_engineGraphicsAdapter->InitializeEngine(hwnd, screenWidth, screenHeight, tResourceListPath);
		_api->Initialize();
		_engineGraphicsAdapter->InitializeGraphics(static_cast<HWND>(hwnd), screenWidth, screenHeight, tResourceListPath);

		//AssetManager ����. (���� ������ ���ҽ� ��� �޾ƿ��� �� �ƴ�, �޾ƿ� ���ҽ� �ϵ��ڵ�!)
		_assetManager->InitializeDefaults();
		_assetManager->Initialize(this, tResourceListPath);
		_engineGraphicsAdapter->UpdateAssetManager(_assetManager);
		//AssetManager ���� ���ҽ� �̸��� ��ġ�� �ʰ� ����.
		_assetManager->AssureNoNameDuplicates();
		_engineGraphicsAdapter->GraphicsConnectDefaultResources();

		//AssetManager���� �ε��� ���ҽ� - �׷��� ������ ����.
		_engineGraphicsAdapter->SyncLoadGraphicsResources();

		return S_OK;
	}

	void ProcessMain::Update()
	{
		//deltaTime ������Ʈ
		//_timeManager->TimeMeasure();
		//_timeManager->MeasureFrame(_timeManager->GetDeltaTime());
		_util->Update();
		//AssetManager ���� ������Ʈ.
		_engineGraphicsAdapter->UpdateAssetManager(_assetManager);

		//������Ʈ�� Renderer-Graphics ���� ����.
		_engineGraphicsAdapter->UpdateRendererChangeList();

		//������ ������Ʈ�Ǳ� ����, �ٲ�������� ������� �׷��Ƚ� ���ҽ��� ��ε������� ����.
		_engineGraphicsAdapter->SyncLoadGraphicsResources();

		//����ٰ� �ý��� �� �� ������Ʈ!!
		_engineGraphicsAdapter->UpdateEngine();

		_engineGraphicsAdapter->PassDebugRenderData();

		_engineGraphicsAdapter->UpdateGraphics(
			_engineGraphicsAdapter->GetCurrentScene(),
			_engineGraphicsAdapter->GetDeltaTime(),
			_engineGraphicsAdapter->GetCameraData());
		
		_api->UpdateLoop();
	}

	void ProcessMain::BeginRender()
	{
		_engineGraphicsAdapter->BeginRender();
	}

	void ProcessMain::Render()
	{
		_engineGraphicsAdapter->Render();
	}

	//Pg::Data::GameObject* ProcessMain::PassPickedObject()
	//{
	//	//GameObject ����. ���� �Լ����� EditorAdapter�� �����ؾ�. Nullptr ���� ����. 
	//	Pg::Data::GameObject* tToPass = _engineGraphicsAdapter->GetPickedGameObjectWithRatios(0.f, 0.f);
	//
	//	return tToPass;
	//}

	void ProcessMain::SetOutlinedObject(Pg::Data::GameObject* obj)
	{
		_engineGraphicsAdapter->SetOutlinedGameObject(obj);
	}

	void ProcessMain::FinalRender()
	{
		_engineGraphicsAdapter->FinalRender();
	}

	void ProcessMain::EndRender()
	{
		_engineGraphicsAdapter->EndRender();

		//����� ���� ������ Ŭ���� (����� �ý���)
		_engineGraphicsAdapter->ClearDebugVectorData();

		//��Ÿ�ӿ� �߰� / ���� / ������ ������Ʈ ����.
		_engineGraphicsAdapter->HandleRenderObjectsRuntime();
	}

	void ProcessMain::Finalize()
	{
		_engineGraphicsAdapter->Finalize();
	}

	void ProcessMain::OnWindowResized(int screenWidth, int screenHeight)
	{
		_engineGraphicsAdapter->OnWindowResized(screenWidth, screenHeight);
	}

	ID3D11Device* ProcessMain::GetGraphicsDevice()
	{
		return _engineGraphicsAdapter->GetGraphicsDevice();
	}

	ID3D11DeviceContext* ProcessMain::GetGraphicsDeviceContext()
	{
		return _engineGraphicsAdapter->GetGraphicsDeviceContext();
	}

	Pg::Core::EngineGraphicsAdapter* ProcessMain::GetEngineGraphicsAdapter()
	{
		return _engineGraphicsAdapter.get();
	}

	Pg::Core::EditorAdapter* ProcessMain::GetEditorAdapter()
	{
		return _editorAdapter.get();
	}

	std::vector<std::string> ProcessMain::GetAssetList(Pg::Data::Enums::eAssetDefine define)
	{
		if (define == Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL) return _assetManager->GetMaterialNamesWithoutDefault();
		else return _assetManager->GetResourcesNameByDefine(define);
	}

	void ProcessMain::OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		//IEngine���� ������ ���̴�. PgProject ������ ������ ������ ���� ���ǿ� �ʼ������� ����.
		//�ᱹ TotalGameManager�� ���� ������ �� �־�� �Ѵ�.
		//SceneSystem���� Project�� ����Ǿ� �ִ� ����Ʈ�� ���� �� �ְ� �Ѵ�.
		//GetProjectSceneList(); �̷� ���� �ִ� ��.
		_engineGraphicsAdapter->OnParagonProjectLoad(sceneVec);
	}
}