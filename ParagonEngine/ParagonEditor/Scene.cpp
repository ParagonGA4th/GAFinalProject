#include "Scene.h"
#include "Event.h"
#include "DataContainer.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Image.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/Transform.h"

#include "../ParagonMath/PgMath.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Scene::Scene()
	:_winName("Scene"), _isShow(true), _isDisable(false)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
	
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	_gizmoType = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Scene::~Scene()
{

}

void Pg::Editor::Window::Scene::Initialize()
{
	_widgetCon->CreateWidget<Pg::UI::Widget::Image>(_dataContainer->GetSceneTexture(), 1920.f, 1080.f);
	_gizmoType->AddEvent(Pg::Editor::eEventType::_GIZMOTYPE, [&](void* data) { _uiManager->SetGizmoType(data); });
}

void Pg::Editor::Window::Scene::Update()
{
	_uiManager->WindowBegin(_winName);

	_uiManager->BeginDisable(_isDisable);
	_widgetCon->Update();	
	
	if (_dataContainer->GetPickObject() != nullptr)
	{
		_uiManager->SetCameraForGizmo(_dataContainer->GetCurrentScene()->GetMainCamera());
		_uiManager->SetTransformForGizmo(&_dataContainer->GetPickObject()->_transform);
	}

	_uiManager->DrawGizmo();
	if (_isDisable) _uiManager->EndDisable();

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

std::string Pg::Editor::Window::Scene::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Scene::SetDisable(bool disable)
{
	_isDisable = disable;
}

