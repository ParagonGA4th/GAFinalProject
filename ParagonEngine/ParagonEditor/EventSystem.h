#pragma once
#include "EventType.h"

#include <unordered_map>
#include <functional>
#include <any>

namespace Pg::Editor
{
	class EventSystem
	{
		using EventID = eEventType;
		using EventHandler = std::function<void()>;

	public:
		void AddEventHandler(const EventID eventId, const EventHandler& handler)
		{
			_eventHandlers[eventId] = handler;
				//[handler](const std::any& eventData) { handler(std::any_cast<T>(eventData)); };
		}		
		
		void RemoveEventHandler(EventID eventId); 

		void TriggerEvent(EventID eventId);

		//template <typename T>
		//	void TriggerEvent(EventID eventId, const T& eventData)
		//{
		//	auto it = _eventHandlers.find(eventId);
		//	if (it != _eventHandlers.end() && it->second) it->second(eventData);
		//}

	private:
		//std::unordered_map<EventID, std::function<void(const std::any&)>> _eventHandlers;
		std::unordered_map<EventID, std::function<void()>> _eventHandlers;
	};
}


