#include "UtilMain.h"
#include "Log.h"
#include "InputSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Util
{
	void UtilMain::Initialize(float screenWidth, float screenHeight)
	{
		auto& _logger = singleton<Pg::Util::Debug::Log>();
		auto& _tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		_inputSystem = &_tInputSystem;

		_logger.Initialize();
		_inputSystem->Initialize(screenWidth, screenHeight);
		
		_logger.SetLoggerLevel(0);
	}

	void UtilMain::Update()
	{
		_inputSystem->Update();
	}

	void UtilMain::Finalize()
	{

	}
}