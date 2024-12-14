#include "TimeSystem.h"
#include "Log.h"
#include <string>
#include <algorithm>

namespace Pg::Util::Time
{
	TimeSystem::TimeSystem() :
		_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER()),
		_totalTick(0.0f)
	{

	}

	void TimeSystem::Initialize(void* hWnd)
	{
		_hWnd = (HWND)hWnd;

		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);
	}

	void TimeSystem::TimeMeasure()
	{
		QueryPerformanceCounter(&_endTick);

		_int64 elapsed = _endTick.QuadPart - _startTick.QuadPart;
		_deltaTime = (float)elapsed / (float)_frequency.QuadPart;

		_deltaTime = std::clamp<float>(_deltaTime, 0.0f, 0.05f);

		//_deltaTime = 0.016;

		MeasureFrame(_deltaTime);

		if (_deltaTime != 0)
		{
			_frameRate = static_cast<int>(1.f / _deltaTime);
		}

		QueryPerformanceCounter(&_startTick);

		//
		if (_isPausedGame)
		{
			// DeltaTime�� �Ͻ����� ���¶�� _deltaTime�� 0���� ����
			_deltaTime = 0.0f;
		}

		//hWnd�� ��������, 
		//Windows Title�� FPS ��Ͽ����� ���. (�غ��� ���� ��ü�� ��� X)
		{
			//+Frame�� ���ư��ٴ� ǥ��. (�ϵ��ڵ� �Ϻ� ����)
			std::wstring tTitleString = L"FPS : ";
			tTitleString.append(std::to_wstring(_frameRate));
			tTitleString.append(L" // ");

			for (int i = 0; i < tCounter; i++)
			{
				tTitleString += L"|";
			}

			tCounter++;

			if (tCounter > 40)
			{
				tCounter = 0;
			}

			assert(SetWindowTextW(_hWnd, tTitleString.c_str()));
		}

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

	const float* const TimeSystem::GetDeltaTimePointer()
	{
		return &_deltaTime;
	}

	void TimeSystem::SetPauseGame(bool val)
	{
		_isPausedGame = val;
	}
}