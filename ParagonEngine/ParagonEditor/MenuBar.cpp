#include "MenuBar.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/MenuBar.h"
#include "../ParagonUI/Text.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::MenuBar::MenuBar()
	:_winName("Paragon Engine"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	cons = new Pg::UI::WidgetContainer();

}

Pg::Editor::Window::MenuBar::~MenuBar()
{

}

void Pg::Editor::Window::MenuBar::Initialize()
{
	_menuBars.insert({ "File", {}});
	_menuBars.at("File").insert({"Open Scene", &_isOpenScene});
	_menuBars.at("File").insert({"Save Scene", &_isSaveScene});

	cons->CreateWidget<Pg::UI::Widget::Text>("Text");
	cons->CreateWidget<Pg::UI::Widget::MenuBar>(_menuBars);
}

void Pg::Editor::Window::MenuBar::Update()
{
	_uiManager->WindowBegin(_winName, true);
	cons->Update();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::MenuBar::Finalize()
{

}

void Pg::Editor::Window::MenuBar::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::MenuBar::GetShow()
{
	return _isShow;
}