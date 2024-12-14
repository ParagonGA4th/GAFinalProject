#pragma once

#include "IEvent.h"

/// <summary>
/// �������� Event�� ����ϴ� ���õ� && �� ���� �̺�Ʈ ����Ʈ.
/// </summary>

namespace Pg::DataScript
{
	//Enemy �ڽ��� �����ΰ� �׾��� �� ȣ���� ��.
	class Event_EnemyOnSelfDead : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnSelfDead");
	};

	//���� ���������� �Ѿ �� �ִ�. ��� ������ �׾��� ��.
	class Event_EnemyOnAllDead : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnAllDead");
	};
	
	//������ �������Ǿ�� �� ��. 
	class Event_EnemyOnRespawn : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnemyOnRespawn");
	};
}



