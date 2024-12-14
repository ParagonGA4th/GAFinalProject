#include "TweenTimer.h"
#include "TimeSystem.h"
#include <cmath>
#include <limits>
#include <singleton-cpp/singleton.h>

namespace Pg::Util
{
	TweenTimer::TweenTimer()
		: _time(0.f), _isActive(false), _duration(0.f), _delayTime(0.f)
	{
		_deltaTimePointer = singleton<Pg::Util::Time::TimeSystem>().GetDeltaTimePointer();
	}

	TweenTimer::~TweenTimer()
	{

	}

	void TweenTimer::Start()
	{
		_isActive = true;
	}

	void TweenTimer::Update()
	{
		if (_isActive == true)
		{
			//_time += TimeSystem::GetDeltaTime();
			//_time += 0.016f; /// Time 오기 전까지 온 명시적인 CUT.
			_time += (*_deltaTimePointer);

			if (_time > _duration)
			{
				_time = _duration;
			}

			//이건, 만약 KillEarly가 사용되고, 정해진 EarlyKill 시간을 넘기면 죽게 하는 것이다.
			if (_isUseKillEarly && (_time > _floatKillEarlyDuration))
			{
				_isActive = false;
			}

			if (_time > 0)
			{
				_play();
			}

			if (abs(_duration - _time) < std::numeric_limits<float>::epsilon())
			{
				_isActive = false;
			}
		}
	}

	void TweenTimer::ResetSelf()
	{
		_time = 0.f;
		_delayTime = 0.f;
		_duration = 0.f;
		_play = nullptr;
		_isActive = false;
		_isUseKillEarly = false;
		_floatKillEarlyDuration = 0.f;
	}

}
