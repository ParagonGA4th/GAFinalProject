#pragma once

#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시들 && 적 관련 이벤트 리스트.
/// </summary>

namespace Pg::DataScript
{
	//Enemy 자신이 스스로가 죽었을 때 호출할 것.
	class Event_EnemyOnSelfDead : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnSelfDead");
	};

	//다음 스테이지로 넘어갈 수 있다. 모든 적들이 죽었을 때.
	class Event_EnemyOnAllDead : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnAllDead");
	};
	
	//적들이 리스폰되어야 할 때. 
	class Event_EnemyOnRespawn : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnRespawn");
	};
}



