#include "TimeManager.h"

namespace Pg::Core::Time
{
	float TimeManager::_deltaTime = 0.0f;
	int TimeManager::_frameRate = 0;

	TimeManager::TimeManager() :
		_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER())
	{

	}

	void TimeManager::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);
	}

	void TimeManager::TimeMeasure()
	{
		QueryPerformanceCounter(&_endTick);

		_totalTick = (float)(_endTick.QuadPart - _startTick.QuadPart) /
			(float)(_frequency.QuadPart);

		_deltaTime = _totalTick;

		QueryPerformanceCounter(&_startTick);

		PG_TRACE(std::to_string(_deltaTime));
	}

	void TimeManager::MeasureFrame(float deltaTime)
	{
		double totalTime = 0.0;
		int frameCount = 0;

		totalTime += deltaTime;

		if (totalTime >= 1)
		{
			_frameRate = frameCount;
			frameCount = 0;
			totalTime = 0;
		}

		frameCount++;
	}

	float TimeManager::GetDeltaTime()
	{
		return _deltaTime;
	}

	int TimeManager::GetFrameRate()
	{
		return _frameRate;
	}
}

