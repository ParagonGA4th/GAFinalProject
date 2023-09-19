#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"

#include "Scene.h"
#include "Transform.h"
#include "GameObject.h"

#include <string>
#include <windows.h>

namespace Pg::Core
{
	CoreMain::CoreMain() :
		_timeManager(Time::TimeManager::Instance())
	{
		_engine = std::make_unique<Pg::Engine::EngineMain>();
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>();
		_logger = std::make_unique<Pg::Util::Debug::Log>();
	}

	CoreMain::~CoreMain()
	{

	}

	long CoreMain::Initialize(void* hwnd, int screenWidth, int screenHeight)
	{
		//deltaTime 초기화
		_timeManager->Initialize();

		//엔진 초기화
		_engine->Initialize(screenWidth, screenHeight);
		_graphics->Initialize(static_cast<HWND>(hwnd), screenWidth, screenHeight);

		//디버그 초기화
		_logger->Initialize();
		_logger->SetLoggerLevel(0);

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
		_graphics->Render();
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