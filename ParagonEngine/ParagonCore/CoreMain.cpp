#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"


namespace Pg::Core
{
	CoreMain::CoreMain()
	{
		_engine = std::make_unique<Pg::Engine::EngineMain>();
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>();
		_logger = std::make_unique<Pg::Util::Debug::Log>();
	}

	CoreMain::~CoreMain()
	{

	}

	long CoreMain::Initialize(int screenWidth, int screenHeight)
	{
		//엔진 초기화
		_engine->Initialize();
		_graphics->Initialize();

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
		//여기다가 시스템 싹 다 업데이트!!
		_engine->Update();
		_graphics->Update();
	}

	void CoreMain::BeginRender()
	{

	}

	void CoreMain::Render()
	{

	}

	void CoreMain::EndRender()
	{

	}

	void CoreMain::Finalize()
	{

	}
}