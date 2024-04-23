#pragma once
#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시이자, 플레이어의 체력이 변경되었을 때 사용됨.
/// </summary>
namespace Pg::DataScript
{
	class Event_PlayerHealthChange : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("PlayerHealthChangeEvent");
	};
}

