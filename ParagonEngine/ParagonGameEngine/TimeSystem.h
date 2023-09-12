#pragma once
#include "../ParagonGameEngine/EngineDLLExporter.h"
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
		PARAGON_ENGINE_DLL TimeSystem();

		PARAGON_ENGINE_DLL void Initialize();
		PARAGON_ENGINE_DLL void TimeMeasure();
		PARAGON_ENGINE_DLL void MeasureFrame(float deltaTime);

		PARAGON_ENGINE_DLL float GetDeltaTime();
		PARAGON_ENGINE_DLL int GetFrameRate();

	public:
		float _deltaTime = 0.0f;
		int _frameRate = 0;
		float _totalTick;

		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTick;
		LARGE_INTEGER _endTick;
	};
}


