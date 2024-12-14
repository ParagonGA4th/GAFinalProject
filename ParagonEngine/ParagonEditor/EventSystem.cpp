#include "EventSystem.h"

void Pg::Editor::System::EventSystem::AddEventHandler(const EventID eventId, const EventHandler& handler)
{
	_eventHandlers[eventId] = handler;
}

void Pg::Editor::System::EventSystem::AddEventHandler(const EventID eventId, const ValueEventHandler& handler)
{
	_valueEventHandlers[eventId] = handler;
}

void Pg::Editor::System::EventSystem::RemoveEventHandler(EventID eventId)
{
	if (_eventHandlers.find(eventId) != _eventHandlers.end()) _eventHandlers.erase(eventId);
	else
	{
		if (_valueEventHandlers.find(eventId) != _valueEventHandlers.end()) _valueEventHandlers.erase(eventId);
	}
}

void Pg::Editor::System::EventSystem::TriggerEvent(EventID eventId, void* value)
{
	auto it = _eventHandlers.find(eventId);
	if (it != _eventHandlers.end()) it->second();
	else
	{
		auto vit = _valueEventHandlers.find(eventId);
		if (vit != _valueEventHandlers.end()) vit->second(value);
	}
}

