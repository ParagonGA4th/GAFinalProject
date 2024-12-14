#pragma once
#include "IEvent.h"

/// <summary>
/// �������� Event�� ����ϴ� ���õ� && ī�޶� ���� �̺�Ʈ ����Ʈ.
/// </summary>

namespace Pg::DataScript
{
	//�Ϲ����� ���ӿ��� �����ϸ鼭 ���ΰ��� ���󰡾� �� �� ȣ��.
	class Event_CameraOnLerpFollowMode : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_CameraOnLerpFollowMode");
	};

	//������ Path�� ���󰡸鼭 �̵��ؾ� �� �� ȣ��.
	class Event_CameraOnPathFollowMode : public IEvent
	{
		//�ڱ� �ڽ� ���� Event Identifier ���.
		DEFINE_EVENT_IDENTIFIER("Event_CameraOnPathFollowMode");
	};

}



