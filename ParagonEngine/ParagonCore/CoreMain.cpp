#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"

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
		_graphics->Update();
	}

	void CoreMain::BeginRender()
	{
		_graphics->BeginRender();
	}


	void CoreMain::Render()
	{
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