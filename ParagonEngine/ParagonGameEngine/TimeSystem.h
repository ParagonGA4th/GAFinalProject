#pragma once

#include "../ParagonCore/Singleton.h"
#include <windows.h>

/// <summary>
/// ParagonEngine¿« TimeSystem.
/// 2023.09.12
/// </summary>
namespace Pg::Engine::Time
{
	class TimeSystem : public Singleton<TimeSystem>
	{
	public:
		TimeSystem();

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


