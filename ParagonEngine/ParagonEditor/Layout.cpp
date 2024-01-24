#include "Layout.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Layout::Layout()
	:_winName("dockSpace"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;
}

Pg::Editor::Window::Layout::~Layout()
{

}

void Pg::Editor::Window::Layout::Initialize()
{

}

void Pg::Editor::Window::Layout::Update()
{
	_uiManager->WindowDockSpaceBegin(_winName);

	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Layout::Finalize()
{

}

void Pg::Editor::Window::Layout::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Layout::GetShow()
{
	return _isShow;
}
