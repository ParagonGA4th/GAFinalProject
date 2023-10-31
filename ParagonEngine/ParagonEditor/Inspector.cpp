#include "Inspector.h"
#include "../ParagonUI/UIManager.h"

#include "../ParagonUI/Column.h"

#include <singleton-cpp/singleton.h>

// Object Name
// Object Tag
// Active
// Transform
// Component


Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{
	cons.CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	cons.CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", _objName);
	cons.CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	cons.CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", _objTag);

	cons.CreateWidget<Pg::UI::Widget::Layout::Column<2>>(cons.GetColumnWidgets());
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	cons.Update();
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
