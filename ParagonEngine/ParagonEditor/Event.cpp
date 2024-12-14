#include "Event.h"
#include "EventSystem.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Event::Event()
{
	auto& tes = singleton<Pg::Editor::System::EventSystem>();
	_eventSystem = &tes;
}

Pg::Editor::Event::~Event()
{
}

void Pg::Editor::Event::AddEvent(eEventType eventType, std::function<void()> callback)
{
	_eventType = eventType;
	_eventSystem->AddEventHandler(eventType, callback);
}

void Pg::Editor::Event::AddEvent(eEventType eventType, std::function<void(void*)> callback)
{
	_eventType = eventType;
	_eventSystem->AddEventHandler(eventType, callback);
}

void Pg::Editor::Event::RemoveEvent(eEventType eventType)
{
	_eventSystem->RemoveEventHandler(eventType);
}

void Pg::Editor::Event::Invoke(eEventType eventType)
{
	_eventSystem->TriggerEvent(eventType);
}

void Pg::Editor::Event::Invoke(eEventType eventType, void* value)
{
	_eventSystem->TriggerEvent(eventType, value);
}
