#include "TweenTimer.h"
//#include "TimeSystem.h"
#include <cmath>
#include <limits>

namespace Pg::Util
{
	TweenTimer::TweenTimer()
		: _time(0.f), _isActive(false), _duration(0.f), _delayTime(0.f)
	{
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
			_time += 0.016f; /// Time 오기 전까지 온 명시적인 CUT.

			if (_time > _duration)
			{
				_time = _duration;
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
}
