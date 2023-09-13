#pragma once
#include "../ParagonCore/Singleton.h"
#include "../ParagonUtil/Log.h"
#include <windows.h>

/// <summary>
/// ParagonCore¿« TimeSystem.
/// 2023.09.12
/// </summary>
namespace Pg::Core::Time
{
	class TimeManager : public Singleton<TimeManager>
	{
	public:
		TimeManager();

		void Initialize();
		void TimeMeasure();
		void MeasureFrame(float deltaTime);

		static float GetDeltaTime();
		static int GetFrameRate();

	public:
		static float _deltaTime;
		static int _frameRate;
		float _totalTick;

		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTick;
		LARGE_INTEGER _endTick;
	};
}


