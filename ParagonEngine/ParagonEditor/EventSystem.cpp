#include "EventSystem.h"

void Pg::Editor::EventSystem::RemoveEventHandler(EventID eventId)
{
	auto eventHandler = _eventHandlers.find(eventId);
	if (eventHandler != _eventHandlers.end()) _eventHandlers.erase(eventHandler);
}

void Pg::Editor::EventSystem::TriggerEvent(EventID eventId)
{
	auto it = _eventHandlers.find(eventId);
	if (it != _eventHandlers.end() && it->second) it->second();

}

