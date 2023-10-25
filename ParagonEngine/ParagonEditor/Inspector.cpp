#include "Inspector.h"
#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/Panel.h"
#include <singleton-cpp/singleton.h>

// Object Name
// Object Tag
// Active
// Transform
// Component

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true), _panel()
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{
	_panel = _uiManager->CreatePanel(_winName);
}

void Pg::Editor::Window::Inspector::Update()
{
	_panel->Update();
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

void Pg::Editor::Window::Inspector::CreateUI()
{

}

