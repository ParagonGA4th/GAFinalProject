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
			//_time += 0.016f; /// Time ���� ������ �� ������� CUT.
			_time += (*_deltaTimePointer);

			if (_time > _duration)
			{
				_time = _duration;
			}

			//�̰�, ���� KillEarly�� ���ǰ�, ������ EarlyKill �ð��� �ѱ�� �װ� �ϴ� ���̴�.
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
