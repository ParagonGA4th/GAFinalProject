#pragma once
#include "EngineDLLExporter.h"

#include <windows.h>

/// <summary>
/// 변지상의 TimeSystem
/// DeltaTime 및 FPS 출력 담당
/// 2023.09.12
/// </summary>
namespace Pg::Engine::Time
{
	class TimeSystem
	{
	public:
		PARAGON_ENGINE_DLL TimeSystem();

		PARAGON_ENGINE_DLL void Initialize();
		PARAGON_ENGINE_DLL void TimeMeasure();
		PARAGON_ENGINE_DLL void MeasureFrame(float deltaTime);

		PARAGON_ENGINE_DLL float GetDeltaTime();
		PARAGON_ENGINE_DLL int GetFrameRate();

	public:
		float _deltaTime = 0.f; 
		int _frameRate = 0;
		float _totalTick = 0.f;

		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTick;
		LARGE_INTEGER _endTick;

		//내부 계산을 위해
	private:
		double totalTime = 0.0;
		int frameCount = 0;
	};
}

