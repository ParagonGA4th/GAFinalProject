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

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

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
		//로직 관련 정보 세팅.
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
		//deltaTime 초기화
		//_timeManager->Initialize();

		//엔진 초기화
		_util->Initialize();
		_engineGraphicsAdapter->InitializeEngine(screenWidth, screenHeight);
		_api->Initialize();
		_engineGraphicsAdapter->InitializeGraphics(static_cast<HWND>(hwnd), screenWidth, screenHeight);

		//AssetManager 세팅. (현재 씬에서 리소스 목록 받아오는 것 아님, 받아올 리소스 하드코딩!)
		_assetManager->Initialize(this);
		_engineGraphicsAdapter->UpdateAssetManager(_assetManager);
		//AssetManager 내부 리소스 이름이 겹치지 않게 관리.
		_assetManager->AssureNoNameDuplicates();

		//AssetManager에서 로딩된 리소스 - 그래픽 엔진과 연동.
		_engineGraphicsAdapter->SyncLoadGraphicsResources();

		return S_OK;
	}

	void ProcessMain::Update()
	{
		//deltaTime 업데이트
		//_timeManager->TimeMeasure();
		//_timeManager->MeasureFrame(_timeManager->GetDeltaTime());

		//AssetManager 로직 업데이트.
		_engineGraphicsAdapter->UpdateAssetManager(_assetManager);

		//업데이트된 Renderer-Graphics 연동 위해.
		_engineGraphicsAdapter->UpdateRendererChangeList();

		//여기다가 시스템 싹 다 업데이트!!
		_engineGraphicsAdapter->UpdateEngine();

		_engineGraphicsAdapter->PassDebugRenderData();

		_engineGraphicsAdapter->UpdateGraphics(
			_engineGraphicsAdapter->GetCurrentScene(),
			_engineGraphicsAdapter->GetCameraData());

		// 에디터에서 변경 된 씬 정보 -> 게임엔진 
		_engineGraphicsAdapter->SetCurrentScene(_editorAdapter->GetCurrentScene());
	}

	void ProcessMain::BeginRender()
	{
		_engineGraphicsAdapter->BeginRender();
	}

	void ProcessMain::Render()
	{
		_engineGraphicsAdapter->Render();
	}

	Pg::Data::GameObject* ProcessMain::PassPickedObject()
	{
		//GameObject 전달. 동일 함수에서 EditorAdapter로 전달해야. Nullptr 전달 가능. 
		Pg::Data::GameObject* tToPass = _engineGraphicsAdapter->GetPickedGameObjectWithRatios(0.f, 0.f);

		return tToPass;
	}

	void ProcessMain::FinalRender()
	{
		_engineGraphicsAdapter->FinalRender();
	}

	void ProcessMain::EndRender()
	{
		_engineGraphicsAdapter->EndRender();

		//디버그 렌더 데이터 클리어 (디버그 시스템)
		_engineGraphicsAdapter->ClearDebugVectorData();
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
}