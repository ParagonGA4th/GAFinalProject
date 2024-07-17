#include "PgTime.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::API::Time
{
	void PgTime::Initialize()
	{
		auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
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

	void PgTime::SetPauseGame(bool val)
	{
		_timeSystem->SetPauseGame(val);
	}
}