#pragma once

#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시들 && 게임 내부 관련 이벤트 리스트.
/// </summary>

namespace Pg::DataScript
{
	//게임이 시작 / 재시작 되었을 때 사용되는 Event.
	class Event_GameOnStart : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_GameOnStart");
	};

	class Event_OnGamePaused : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnGamePaused");
	};

	class Event_OnGameExit : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnGameExit");
	};

	// 보스 전이 아닌, 일반 게임 영역 (일반 적들)에 들어갈 때.
	class Event_OnNormalGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnNormalGameAreaEnter");
	};

	// 중간 보스 (미니보스) 게임 영역에 들어갈 때.
	class Event_OnMiniBossGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnMiniBossGameAreaEnter");
	};

	// 대형 보스 게임 영역에 들어갈 때.
	class Event_OnBigBossGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnBigBossGameAreaEnter");
	};

	

}



