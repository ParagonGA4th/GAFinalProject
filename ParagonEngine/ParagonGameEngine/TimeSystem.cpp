#include "TimeSystem.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Engine::Time
{
	TimeSystem::TimeSystem() :
		_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER()),
		_totalTick(0.0f)
	{

	}

	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);
	}

	void TimeSystem::TimeMeasure()
	{
		QueryPerformanceCounter(&_endTick);

		_int64 elapsed = _endTick.QuadPart - _startTick.QuadPart;
		_deltaTime = (float)elapsed / (float)_frequency.QuadPart;
		//_deltaTime = 0.016;

		MeasureFrame(_deltaTime);

		if (_deltaTime != 0)
		{
			_frameRate = static_cast<double>(1) / _deltaTime;
		}

		QueryPerformanceCounter(&_startTick);
	}

	void TimeSystem::MeasureFrame(float deltaTime)
	{
		totalTime += deltaTime;

		if (totalTime >= 1)
		{
			_frameRate = frameCount;
			totalTime = 0;
			frameCount = 0;
		}

		++frameCount;
	}

	float TimeSystem::GetDeltaTime()
	{
		return  _deltaTime;
	}

	int TimeSystem::GetFrameRate()
	{
		return _frameRate;
	}

}