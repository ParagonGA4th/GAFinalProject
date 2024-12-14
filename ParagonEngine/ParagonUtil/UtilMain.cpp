#include "UtilMain.h"
#include "Log.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "InstancingException.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Util
{
	void UtilMain::Initialize(const std::string& resourceListPath, float screenWidth, float screenHeight, void* hwnd)
	{
		auto& _logger = singleton<Pg::Util::Debug::Log>();
		
		//Input
		_inputSystem = &singleton<Pg::Util::Input::InputSystem>();

		//Time
		_timeSystem = &singleton<Time::TimeSystem>();

		//InstException
		_instancingException = &singleton<InstancingException>();

		//Initializing..
		_logger.Initialize();
		_inputSystem->Initialize(screenWidth, screenHeight);

		_logger.SetLoggerLevel(0);

		_timeSystem->Initialize(hwnd);

		_instancingException->Initialize(resourceListPath);
	}

	void UtilMain::Update()
	{
		///업데이트 순서 무조건 고정!!! 바뀌면 안됨
		_timeSystem->TimeMeasure();

		_inputSystem->Update();

		
	}

	void UtilMain::Finalize()
	{
		
	}
}