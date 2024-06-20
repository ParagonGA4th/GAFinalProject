#include "Layout.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/MenuBar.h"

#include <map>
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
	std::map<std::string, std::map<std::string, bool*>> menubar;

	menubar.insert({ "0 File", {} });
	//menubar.at("0 File").insert({ "0 New Scene (Ctrl + N)", &_isNewScene });
	menubar.at("0 File").insert({ "1 Open Scene (Ctrl + O)", &_isOpenScene });
	menubar.at("0 File").insert({ "2 Save Scene (Ctrl + S)", &_isSaveScene });
	menubar.at("0 File").insert({ "3 Line", nullptr });
	//menubar.at("0 File").insert({ "4 New Project", &_isNewProj });
	menubar.at("0 File").insert({ "5 Open Project", &_isOpenProj });
	//menubar.at("0 File").insert({ "6 Save Project", &_isSaveProj });

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
	if (_isNewScene) 
	{
		_fileState->Invoke(eEventType::_NEWSCENE); 
		_isNewScene = false;
	}

	if (_isOpenScene)
	{
		_fileState->Invoke(eEventType::_OPENSCENE); 
		_isOpenScene = false;
	}

	if (_isSaveScene)
	{
		_fileState->Invoke(eEventType::_SAVESCENE); 
		_isSaveScene = false;
	}

	if (_isNewProj)
	{
		_fileState->Invoke(eEventType::_NEWPROJECT);
		_isNewProj = false;
	}

	if (_isOpenProj)
	{
		_fileState->Invoke(eEventType::_OPENPROJECT); 
		_isOpenProj = false;
	}

	if (_isSaveProj)
	{
		_fileState->Invoke(eEventType::_SAVEPROJECT); 
		_isSaveProj = false;
	}
}

std::string Pg::Editor::Window::Layout::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Layout::SetDisable(bool disable)
{

}
