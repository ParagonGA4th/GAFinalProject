#include "Inspector.h"
#include "InspectorUIManager.h"
#include "InspectorDataManager.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_insDataManager = std::make_unique<Pg::Editor::Window::InspectorDataManager>();
	_insUIManager = std::make_unique<Pg::Editor::Window::InspectorUIManager>();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{
	_insDataManager->Initialize(_insUIManager.get());
	_insUIManager->Initialize(_insDataManager.get(), _uiManager);

	std::unique_ptr<Pg::Editor::Event> changeObjectData = std::make_unique<Pg::Editor::Event>();
	changeObjectData->AddEvent(Pg::Editor::eEventType::_OBJECTDATA, [&](void* data) { SetData(data); });
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	_uiManager->BeginDisable(_isDisable);

	//_insDataManager->SetFlag(_uiManager->IsWindowFocus(_winName));
	_insDataManager->Update();
	_insUIManager->Update();

	if (_isDisable) _uiManager->EndDisable();
	
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Inspector::Finalize()
{

}

void Pg::Editor::Window::Inspector::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Inspector::GetShow()
{
	return _isShow;
}

void Pg::Editor::Window::Inspector::SetData(void* data)
{
	if (data != nullptr)
	{
		_selectObject = static_cast<Pg::Data::GameObject*>(data);
		_insDataManager->SetObject(_selectObject);
	}
}

std::string Pg::Editor::Window::Inspector::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Inspector::SetDisable(bool disable)
{
	_isDisable = disable;
}


