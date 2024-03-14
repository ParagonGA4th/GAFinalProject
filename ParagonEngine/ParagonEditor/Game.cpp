#include "Game.h"
#include "DataContainer.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Image.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Game::Game()
	:_winName("Game"), _isShow(true), _isDisable(false)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

}

Pg::Editor::Window::Game::~Game()
{

}

void Pg::Editor::Window::Game::Initialize()
{
	_widgetCon->CreateWidget<Pg::UI::Widget::Image>(_dataContainer->GetGameTexture(), 1920.f, 1080.f);
}

void Pg::Editor::Window::Game::Update()
{
	_uiManager->WindowBegin(_winName);
	_widgetCon->Update();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Game::Finalize()
{

}

void Pg::Editor::Window::Game::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Game::GetShow()
{
	return _isShow;
}

std::string Pg::Editor::Window::Game::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Game::SetDisable(bool disable)
{
	_isDisable = disable;
}

