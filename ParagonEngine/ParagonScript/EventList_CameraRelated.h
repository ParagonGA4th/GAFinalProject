#pragma once
#include "IEvent.h"

/// <summary>
/// 개별적인 Event를 등록하는 예시들 && 카메라 관련 이벤트 리스트.
/// </summary>

namespace Pg::DataScript
{
	//일반적인 게임에서 보간하면서 주인공을 따라가야 할 때 호출.
	class Event_CameraOnLerpFollowMode : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_CameraOnLerpFollowMode");
	};

	//정해진 Path를 따라가면서 이동해야 할 때 호출.
	class Event_CameraOnPathFollowMode : public IEvent
	{
		//자기 자신 기준 Event Identifier 등록.
		DEFINE_EVENT_IDENTIFIER("Event_CameraOnPathFollowMode");
	};

}



