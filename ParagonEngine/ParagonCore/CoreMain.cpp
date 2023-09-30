#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonUtil/UtilMain.h"
#include "../ParagonAPI/APIMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "CameraData.h"
#include "Scene.h"
#include "Transform.h"
#include "GameObject.h"
#include "AssetManager.h"

#include <string>
#include <windows.h>
#include <singleton-cpp/singleton.h>

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
	CoreMain::CoreMain()
	{
		auto& tAssetManager = singleton<Manager::AssetManager>();
		_assetManager = &tAssetManager;

		auto& timeSystem = singleton<Time::TimeManager>();
		_timeManager = &timeSystem;

		_engine = std::make_unique<Pg::Engine::EngineMain>(this);
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>(this);
		//_logger = std::make_unique<Pg::Util::Debug::Log>();
		_util = std::make_unique<Pg::Util::UtilMain>();
		_api = std::make_unique<Pg::API::APIMain>();
	}

	CoreMain::~CoreMain()
	{

	}

	long CoreMain::Initialize(void* hwnd, int screenWidth, int screenHeight)
	{
		//deltaTime 초기화
		_timeManager->Initialize();

		//엔진 초기화
		_util->Initialize();
		_api->Initialize();
		_engine->Initialize(screenWidth, screenHeight);
		_graphics->Initialize(static_cast<HWND>(hwnd), screenWidth, screenHeight);

		//AssetManager 세팅.
		_assetManager->Initialize(this);

		_work = new Pg::Engine::WorkSpace();
		_work->Initialize();

		PG_TRACE("Engine Success!!");
		PG_DEBUG("Engine Success!!");
		PG_INFO("Engine Success!!");
		PG_WARN("Engine Success!!");

		return S_OK;
	}

	void CoreMain::Update()
	{
		//deltaTime 업데이트
		_timeManager->TimeMeasure();
		_timeManager->MeasureFrame(_timeManager->GetDeltaTime());

		//AssetManager 로직 업데이트.
		_assetManager->Update(_engine.get(), _graphics.get());

		//여기다가 시스템 싹 다 업데이트!!
		_engine->Update();

		CameraData cameraData;
		cameraData._position = { 0.0f, 0.0f, -3.0f };
		cameraData._rotation.x = 0.0f;
		cameraData._rotation.y = 0.0f;
		cameraData._rotation.z = 0.0f;
		cameraData._rotation.w = 0.0f;

		cameraData._viewMatrix =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 3.0f, 1.0f,
		};

		cameraData._projMatrix =
		{
			1.35799503f, 0.0f, 0.0f, 0.0f,
			0.0f, 2.41421342f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.00000012f, 1.0f,
			0.0f, 0.0f, -0.000100000012f, 0.0f,
		};

		_graphics->Update(_work->GetCurrentScene(), cameraData);
	}

	void CoreMain::BeginRender()
	{
		_graphics->BeginRender();
	}


	void CoreMain::Render()
	{
		//Render(_engine->GetCurrentScene());
		_graphics->Render(_work->GetCurrentScene());
	}


	void CoreMain::EndRender()
	{
		_graphics->EndRender();
	}


	void CoreMain::Finalize()
	{
		_graphics->Finalize();
	}

	void CoreMain::OnWindowResized(int screenWidth, int screenHeight)
	{
		_graphics->OnWindowResized(screenWidth, screenHeight);
	}

	ID3D11Device* CoreMain::GetGraphicsDevice()
	{
		return _graphics->GetDevice();
	}

	ID3D11DeviceContext* CoreMain::GetGraphicsDeviceContext()
	{
		return _graphics->GetDeviceContext();
	}

}