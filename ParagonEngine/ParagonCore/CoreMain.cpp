#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"

namespace Pg::Core
{
	CoreMain::CoreMain()
	{
		
	}

	void CoreMain::Initialize()
	{
		Pg::Engine::EngineMain* engine = new Pg::Engine::EngineMain();

		Pg::Graphics::GraphicsMain* graphics = new Pg::Graphics::GraphicsMain();

		engine->Initialize();
		graphics->Initialize();
	}

	void CoreMain::Finalize()
	{

	}

	

}