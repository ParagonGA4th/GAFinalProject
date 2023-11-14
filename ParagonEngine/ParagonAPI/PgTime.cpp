#include "PgTime.h"
#include "../ParagonGameEngine/TimeSystem.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::Time
{
	void PgTime::Initialize()
	{
		auto& tTimeSystem = singleton<Pg::Engine::Time::TimeSystem>();
		_timeSystem = &tTimeSystem;
	}

	float PgTime::GetDeltaTime()
	{
		return _timeSystem->GetDeltaTime();
	}

	float PgTime::GetFrameRate()
	{
		return _timeSystem->GetFrameRate();
	}
}