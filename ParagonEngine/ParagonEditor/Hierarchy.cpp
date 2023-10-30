#include "Hierarchy.h"
#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include <singleton-cpp/singleton.h>

#include "../ParagonUI/Selectable.h"

#include <vector>

Pg::Editor::Window::Hierarchy::Hierarchy()
	:_winName("Hierarchy"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	cons = new Pg::UI::WidgetContainer();
}

Pg::Editor::Window::Hierarchy::~Hierarchy()
{

}

void Pg::Editor::Window::Hierarchy::Initialize()
{
	std::vector<std::string> test;

	test.push_back("TestObj_1");
	test.push_back("TestObj_2");
	test.push_back("TestObj_3");

	cons->CreateWidget<Pg::UI::Widget::Selectable>(test);
}

void Pg::Editor::Window::Hierarchy::Update()
{
	_uiManager->WindowBegin(_winName);
	cons->Update();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Hierarchy::Finalize()
{

}

void Pg::Editor::Window::Hierarchy::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Hierarchy::GetShow()
{
	return _isShow;
}
