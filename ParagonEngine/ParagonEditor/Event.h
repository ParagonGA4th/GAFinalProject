#pragma once
#include "EventType.h"
#include <windows.h>
#include <functional>

namespace Pg::Editor
{
	class EventSystem;
	class Event
	{
	public:
		Event();
		~Event();

		void AddEvent(eEventType eventType, std::function<void()> callback);
		void RemoveEvent(eEventType eventType);
		void Invoke(eEventType eventType = eEventType::NONE);

		void EventHandler(MSG message);

	private:
		eEventType _eventType;
		EventSystem* _eventSystem;
	};
}


