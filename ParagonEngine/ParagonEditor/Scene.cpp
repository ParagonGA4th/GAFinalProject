#include "Scene.h"
#include "DataContainer.h"
#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include <singleton-cpp/singleton.h>

#include "../ParagonUI/Image.h"

Pg::Editor::Window::Scene::Scene()
	:_winName("Scene"), _isShow(true)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
	
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	cons = new Pg::UI::WidgetContainer();
}

Pg::Editor::Window::Scene::~Scene()
{

}

void Pg::Editor::Window::Scene::Initialize()
{
	cons->CreateWidget<Pg::UI::Widget::Image>(_dataContainer->GetSceneTexture(), 1080.f, 920.f);
}

void Pg::Editor::Window::Scene::Update()
{
	_uiManager->WindowBegin(_winName);
	cons->Update();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Scene::Finalize()
{

}

void Pg::Editor::Window::Scene::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Scene::GetShow()
{
	return _isShow;
}


