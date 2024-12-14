#pragma once
#include "IEvent.h"

/// <summary>
/// �������� Event�� ����ϴ� ���õ� && �÷��̾��� �̺�Ʈ ����Ʈ.
/// </summary>

namespace Pg::DataScript
{
	//�÷��̾��� ü���� ���� �� ����.
	class Event_PlayerOnLowHealth : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerOnLowHealth");
	};

	class Event_PlayerDeath : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerDeath");
	};

	class Event_PlayerGetArtifact : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_PlayerGetArtifact");
	};

	class Event_UI_SetActiveSkill : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_UI_SetActiveSkill");
	};	
	
}



