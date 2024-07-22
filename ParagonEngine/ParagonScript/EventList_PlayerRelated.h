#pragma once
#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시들 && 플레이어의 이벤트 리스트.
/// </summary>

namespace Pg::DataScript
{
	//플레이어의 체력이 낮을 때 사용됨.
	class Event_PlayerOnLowHealth : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerOnLowHealth");
	};

	class Event_PlayerDeath : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerDeath");
	};

	class Event_PlayerGetArtifact : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerGetArtifact");
	};

	class Event_UI_SetActiveSkill : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_UI_SetActiveSkill");
	};	
	
}



