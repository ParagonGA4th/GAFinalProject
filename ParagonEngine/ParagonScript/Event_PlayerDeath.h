#pragma once
#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시이자, 플레이어가 죽었을 때 보내는 이벤트로 활용.
/// </summary>
namespace Pg::DataScript
{
	class Event_PlayerDeath : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("PlayerDeathEvent");
	};
}

