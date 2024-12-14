#include "ToolBar.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Button.h"
#include "../ParagonUI/RadioButton.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::ToolBar::ToolBar()
	:_winName("ToolBar"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	_editorManaged = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::ToolBar::~ToolBar()
{

}

void Pg::Editor::Window::ToolBar::Initialize()
{
	_widgetCon->CreateWidget<Pg::UI::Widget::Button>("Start", 80.f, 25.f, _isStartBtnClick);
	_widgetCon->CreateWidget<Pg::UI::Widget::Button>("Stop", 80.f, 25.f, _isStopBtnClick);
	_widgetCon->CreateWidget<Pg::UI::Widget::RadioButton>("Translate", _gizmoType);
	_widgetCon->CreateWidget<Pg::UI::Widget::RadioButton>("Rotate", _gizmoType);
	_widgetCon->CreateWidget<Pg::UI::Widget::RadioButton>("Scale", _gizmoType);
}

void Pg::Editor::Window::ToolBar::Update()
{
	_uiManager->WindowBegin(_winName);

	_uiManager->SetAlignCenter(2, 80.0f);
	_widgetCon->SameLine(true, 1);
	_widgetCon->Update();

	if (_isStartBtnClick)
	{
		_editorModeType = Pg::Data::Enums::eEditorMode::_PLAY;
		_editorManaged->Invoke(eEventType::_EDITORMODE, static_cast<void*>(&_editorModeType));
		_editorManaged->Invoke(eEventType::_EDITORDISABLE, static_cast<void*>(&_isStartBtnClick));
		_editorManaged->Invoke(eEventType::_REFRESHOBJECT, static_cast<void*>(&_isStartBtnClick));
	}

	if (_isStopBtnClick)
	{
		_editorModeType = Pg::Data::Enums::eEditorMode::_EDIT;
		_editorManaged->Invoke(eEventType::_EDITORMODE, static_cast<void*>(&_editorModeType));
		_editorManaged->Invoke(eEventType::_EDITORDISABLE, static_cast<void*>(&_isStartBtnClick));
		_editorManaged->Invoke(eEventType::_REFRESHOBJECT, static_cast<void*>(&_isStopBtnClick));
	}

	if (_prevGizmoType.compare(_gizmoType) == -1) _editorManaged->Invoke(eEventType::_GIZMOTYPE, static_cast<void*>(&_gizmoType));
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::ToolBar::Finalize()
{

}

void Pg::Editor::Window::ToolBar::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::ToolBar::GetShow()
{
	return _isShow;
}

std::string Pg::Editor::Window::ToolBar::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::ToolBar::SetDisable(bool disable)
{

}