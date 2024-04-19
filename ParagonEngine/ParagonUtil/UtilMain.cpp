#include "UtilMain.h"
#include "Log.h"
#include "InputSystem.h"
#include "TweenSystem.h"
#include "TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Util
{
	void UtilMain::Initialize(float screenWidth, float screenHeight, void* hwnd)
	{
		auto& _logger = singleton<Pg::Util::Debug::Log>();
		
		//Input
		_inputSystem = &singleton<Pg::Util::Input::InputSystem>();

		//Time
		_timeSystem = &singleton<Time::TimeSystem>();
		
		//Tween
		_tweenSystem = &singleton<TweenSystem>();
		
		//Initializing..
		_logger.Initialize();
		_inputSystem->Initialize(screenWidth, screenHeight);

		_logger.SetLoggerLevel(0);

		_timeSystem->Initialize(hwnd);

		_tweenSystem->Initialize();
	}

	void UtilMain::Update()
	{
		///업데이트 순서 무조건 고정!!! 바뀌면 안됨
		_timeSystem->TimeMeasure();

		_inputSystem->Update();

		_tweenSystem->Update();
	}

	void UtilMain::Finalize()
	{
		_tweenSystem->Finalize();
	}
}