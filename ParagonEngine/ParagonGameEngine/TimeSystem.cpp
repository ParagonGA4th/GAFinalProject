#include "TimeSystem.h"

namespace Pg::Engine::Time
{
	float TimeSystem::_deltaTime = 0.0f;
	int TimeSystem::_frameRate = 0;

	TimeSystem::TimeSystem() :
		_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER())
	{

	}

	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);
	}

	void TimeSystem::MeasureFrame(float deltaTime)
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

	///deltaTime ÃøÁ¤
	void TimeSystem::TimeMeasure()
	{
		QueryPerformanceCounter(&_endTick);

		_totalTick = (float)(_endTick.QuadPart - _startTick.QuadPart) /
			(float)(_frequency.QuadPart);

		_deltaTime = _totalTick;

		QueryPerformanceCounter(&_startTick);
	}

	float TimeSystem::GetDeltaTime()
	{
		return _deltaTime;
	}

	int TimeSystem::GetFrameRate()
	{
		return _frameRate;
	}
}