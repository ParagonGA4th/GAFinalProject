#include "ToolBar.h"
#include "DataContainer.h"
#include "Event.h"

#include "GizmoType.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Button.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::ToolBar::ToolBar()
	:_winName("ToolBar"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	_editorManaged = std::make_unique<Pg::Editor::Event>();

	_editorModeType = new Pg::Data::Enums::eEditorMode();
}

Pg::Editor::Window::ToolBar::~ToolBar()
{

}

void Pg::Editor::Window::ToolBar::Initialize()
{
	auto& startBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Start", 80.f, 25.f);
	_isStartBtnClick = startBtn.GetBtnClick();

	auto& pauseBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Pause", 80.f, 25.f);
	_isPauseBtnClick = pauseBtn.GetBtnClick();

	auto& stopBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Stop", 80.f, 25.f);
	_isStopBtnClick = stopBtn.GetBtnClick();

	auto& transBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Translate", 80.f, 25.f);
	_isTransBtnClick = transBtn.GetBtnClick();
	
	auto& rotBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Rotate", 80.f, 25.f);
	_isRotateBtnClick = rotBtn.GetBtnClick();
		
	auto& scaleBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Scale", 80.f, 25.f);
	_isScaleBtnClick = scaleBtn.GetBtnClick();
}

void Pg::Editor::Window::ToolBar::Update()
{
	_uiManager->WindowBegin(_winName);

	_uiManager->SetAlignCenter(3, 80.0f);
	_widgetCon->SameLine(true, 2);
	_widgetCon->Update();

	if (*_isStartBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_PLAY;
		_editorManaged->Invoke(eEventType::_EDITORMODE, _editorModeType);
		_editorManaged->Invoke(eEventType::_EDITORDISABLE, static_cast<void*>(_isStartBtnClick));
	}

	if (*_isPauseBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_PAUSE;
		_editorManaged->Invoke(eEventType::_EDITORMODE, _editorModeType);
	}	
	
	if (*_isStopBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_EDIT;
		_editorManaged->Invoke(eEventType::_EDITORMODE, _editorModeType);

		*_isStopBtnClick = false;
		_editorManaged->Invoke(eEventType::_EDITORDISABLE, static_cast<void*>(_isStartBtnClick));
	}	
	
	if (*_isTransBtnClick)
	{
		_type = eGizmoType::TRANSLATE;
		_editorManaged->Invoke(eEventType::_GIZMOTYPE, static_cast<void*>(&_type));
	}

	if (*_isRotateBtnClick)
	{
		_type = eGizmoType::ROTATE;
		_editorManaged->Invoke(eEventType::_GIZMOTYPE, static_cast<void*>(&_type));
	}

	if (*_isScaleBtnClick)
	{
		_type = eGizmoType::SCALE;
		_editorManaged->Invoke(eEventType::_GIZMOTYPE, static_cast<void*>(&_type));
	}

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
