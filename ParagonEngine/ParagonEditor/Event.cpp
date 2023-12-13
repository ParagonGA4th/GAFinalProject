#include "Event.h"
#include "EventSystem.h"
#include "EditorDefine.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Event::Event()
{
	auto& tes = singleton<Pg::Editor::EventSystem>();
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

void Pg::Editor::Event::RemoveEvent(eEventType eventType)
{
	_eventSystem->RemoveEventHandler(eventType);
}

void Pg::Editor::Event::Invoke(eEventType eventType)
{
	_eventSystem->TriggerEvent(eventType == eEventType::NONE ? _eventType : eventType);
}

void Pg::Editor::Event::EventHandler(MSG message)
{
	if (message.message == WM_COMMAND)
	{
		switch (LOWORD(message.wParam))
		{
		case ID_OPEN_PROJECT:
			break;

		case ID_NEW_PROJECT:
			break;

		case ID_OPEN_SCENE:
			Invoke(eEventType::FileOpen);
			break;

		case ID_NEW_SCENE:
			break;

		case ID_SAVE:
			Invoke(eEventType::FileSave);
			break;
		}
	}

}

