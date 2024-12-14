#pragma once
#include "EventType.h"

#include <unordered_map>
#include <functional>

namespace Pg::Editor::System
{
	class EventSystem
	{
		using EventID = eEventType;
		using EventHandler = std::function<void()>;
		using ValueEventHandler = std::function<void(void*)>;

	public:
		void AddEventHandler(const EventID eventId, const EventHandler& handler);
		void AddEventHandler(const EventID eventId, const ValueEventHandler& handler);
		
		void RemoveEventHandler(EventID eventId); 

		void TriggerEvent(EventID eventId, void* value = nullptr);
	private:
		std::unordered_map<EventID, std::function<void()>> _eventHandlers;
		std::unordered_map<EventID, std::function<void(void*)>> _valueEventHandlers;
	};
}


