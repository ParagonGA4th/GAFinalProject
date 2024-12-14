#pragma once
#include "UtilDLLExporter.h"

#include <windows.h>
#include <string>

/// <summary>
/// 변지상의 TimeSystem
/// DeltaTime 및 FPS 출력 담당
/// 2023.09.12
/// </summary>

namespace Pg::Util::Time
{
	class TimeSystem
	{
	public:
		PARAGON_UTIL_DLL TimeSystem();
				
		PARAGON_UTIL_DLL void Initialize(void* hWnd);
		PARAGON_UTIL_DLL void TimeMeasure();
		PARAGON_UTIL_DLL void MeasureFrame(float deltaTime);
				
		PARAGON_UTIL_DLL float GetDeltaTime();
		PARAGON_UTIL_DLL int GetFrameRate();

		//일괄적으로 델타타임의 포인터를 받아서 갖다가 쓸 수 있게. 변경 불가하게 const * const 처리.
		PARAGON_UTIL_DLL const float* const GetDeltaTimePointer();
		PARAGON_UTIL_DLL void SetPauseGame(bool val);

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

		//게임 일시정지를 위해.
		bool _isPausedGame = false;

		HWND _hWnd;

	private:
		//전혀 실제 기능에는 영향 X, 비주얼을 위해
		int tCounter = 0;
	};
}

