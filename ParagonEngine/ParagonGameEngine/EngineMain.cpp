#include "EngineMain.h"
#include <windows.h>
#include <cassert>

namespace Pg::Engine
{
	EngineMain::EngineMain() :
		_timeSystem(Time::TimeSystem::Instance())
	{

	}

	void EngineMain::Initialize()
	{
		_timeSystem->Initialize();
	}

	void EngineMain::Update()
	{
		_timeSystem->TimeMeasure();
		//PG_TRACE(std::to_string(_timeSystem->GetDeltaTime()));
	}

	void EngineMain::Finalize()
	{

	}
}
