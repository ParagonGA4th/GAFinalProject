#pragma once

#include "IEvent.h"

/// <summary>
/// �������� Event�� ����ϴ� ���õ� && ���� ���� ���� �̺�Ʈ ����Ʈ.
/// </summary>

namespace Pg::DataScript
{
	//������ ���� / ����� �Ǿ��� �� ���Ǵ� Event.
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

	// ���� ���� �ƴ�, �Ϲ� ���� ���� (�Ϲ� ����)�� �� ��.
	class Event_OnNormalGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnNormalGameAreaEnter");
	};

	// �߰� ���� (�̴Ϻ���) ���� ������ �� ��.
	class Event_OnGolemBossGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnGolemBossGameAreaEnter");
	};

	// ���� ���� ���� ������ �� ��.
	class Event_OnFinalBossGameAreaEnter : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnFinalBossGameAreaEnter");
	};

	class Event_EnableJump : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_EnableJump");
	};

	class Event_OnFinalBossDeathGameWin : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnFinalBossDeathGameWin");
	};

	class Event_DisableJump : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_DisableJump");
	};

	class Event_OnGameOver : public IEvent
	{
		DEFINE_EVENT_IDENTIFIER("Event_OnGameOver");
	};
}



