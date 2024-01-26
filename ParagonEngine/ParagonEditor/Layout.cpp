#include "Layout.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/MenuBar.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Layout::Layout()
	:_winName("dockSpace"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();
	_fileState = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Layout::~Layout()
{

}

void Pg::Editor::Window::Layout::Initialize()
{
	std::unordered_map<std::string, std::unordered_map<std::string, bool*>> menubar;

	menubar.insert({ "File", {} });
	menubar.at("File").insert({"Open Project", &_isOpen});
	menubar.at("File").insert({"Save Project", &_isSave});

	_widgetCon->CreateWidget<Pg::UI::Widget::MenuBar>(menubar);
}

void Pg::Editor::Window::Layout::Update()
{
	_uiManager->WindowDockSpaceBegin(_winName);
	_widgetCon->Update();
	FileUpdate();
	_uiManager->WindowEnd(true);
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

void Pg::Editor::Window::Layout::FileUpdate()
{
	if (_isOpen) _fileState->Invoke(eEventType::_FILEOPEN); _isOpen = false;
	if (_isSave) _fileState->Invoke(eEventType::_FILESAVE); _isSave = false;
}
