#pragma once
#include "EventType.h"

#include <functional>
#include <windows.h>

namespace Pg::Editor
{
	class EventSystem;
	class Event
	{
	public:
		Event();
		~Event();

		void AddEvent(eEventType eventType, std::function<void()> callback);
		void AddEvent(eEventType eventType, std::function<void(void*)> callback);
		void RemoveEvent(eEventType eventType);
		void Invoke(eEventType eventType);
		void Invoke(eEventType eventType, void* value);

		void EventHandler(MSG message);

	private:
		eEventType _eventType;
		EventSystem* _eventSystem;
	};
}
