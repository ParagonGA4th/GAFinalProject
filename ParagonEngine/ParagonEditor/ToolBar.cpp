#include "ToolBar.h"
#include "DataContainer.h"
#include "Event.h"

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
	_editorMode = std::make_unique<Pg::Editor::Event>();

	_editorModeType = new Pg::Data::Enums::eEditorMode();
}

Pg::Editor::Window::ToolBar::~ToolBar()
{

}

void Pg::Editor::Window::ToolBar::Initialize()
{
	auto& startBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Start", 50.f, 25.f);
	_isStartBtnClick = startBtn.GetBtnClick();

	auto& pauseBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Pause", 50.f, 25.f);
	_isPauseBtnClick = pauseBtn.GetBtnClick();

	auto& stopBtn = _widgetCon->CreateWidget<Pg::UI::Widget::Button>("Stop", 50.f, 25.f);
	_isStopBtnClick = stopBtn.GetBtnClick();
}

void Pg::Editor::Window::ToolBar::Update()
{
	_uiManager->WindowBegin(_winName);
	_uiManager->SetAlignCenter(3, 50.0f);
	_widgetCon->SameLine(true);
	_widgetCon->Update();

	if (*_isStartBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_PLAY;
		_editorMode->Invoke(eEventType::_EDITORMODE, _editorModeType);
	}

	if (*_isPauseBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_PAUSE;
		_editorMode->Invoke(eEventType::_EDITORMODE, _editorModeType);
	}	
	
	if (*_isStopBtnClick)
	{
		*_editorModeType = Pg::Data::Enums::eEditorMode::_EDIT;
		_editorMode->Invoke(eEventType::_EDITORMODE, _editorModeType);
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