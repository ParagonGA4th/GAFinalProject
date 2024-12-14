#pragma once
#include "UtilDLLExporter.h"

#include <windows.h>
#include <string>

/// <summary>
/// �������� TimeSystem
/// DeltaTime �� FPS ��� ���
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

		//�ϰ������� ��ŸŸ���� �����͸� �޾Ƽ� ���ٰ� �� �� �ְ�. ���� �Ұ��ϰ� const * const ó��.
		PARAGON_UTIL_DLL const float* const GetDeltaTimePointer();
		PARAGON_UTIL_DLL void SetPauseGame(bool val);

	public:
		float _deltaTime = 0.f; 
		int _frameRate = 0;
		float _totalTick = 0.f;

		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTick;
		LARGE_INTEGER _endTick;

		//���� ����� ����
	private:
		double totalTime = 0.0;
		int frameCount = 0;

		//���� �Ͻ������� ����.
		bool _isPausedGame = false;

		HWND _hWnd;

	private:
		//���� ���� ��ɿ��� ���� X, ���־��� ����
		int tCounter = 0;
	};
}

