#include "TimeManager.h"

namespace Pg::Core::Time
{
	float TimeManager::_deltaTime = 0.0f;
	int TimeManager::_frameRate = 0;

	TimeManager::TimeManager() :
		_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER()),
		_totalTick(0.0f)
	{

	}

	void TimeManager::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);
	}

	void TimeManager::MeasureFrame(float deltaTime)
	{
		static double totalTime = 0.0;
		static int frameCount = 0;


		totalTime += deltaTime;

		if (totalTime >= 1)
		{
			_frameRate = frameCount;
			totalTime = 0;
			frameCount = 0;
		}

		++frameCount;

	}


	void TimeManager::TimeMeasure()
	{
		QueryPerformanceCounter(&_endTick);

		_int64 elapsed = _endTick.QuadPart - _startTick.QuadPart;
		_deltaTime = (float)elapsed / (float)_frequency.QuadPart;
		//_deltaTime = 0.016;

		MeasureFrame(_deltaTime);

		if (_deltaTime != 0)
		{
			_frameRate = static_cast<double>(1)/ _deltaTime;
		}

		QueryPerformanceCounter(&_startTick);
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

