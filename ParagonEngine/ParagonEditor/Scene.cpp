#include "Scene.h"
#include "Event.h"
#include "DataContainer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameConstantData.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Image.h"

#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Scene::Scene()
	:_winName("Scene"), _isShow(true), _isDisable(false)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	_mousePos = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Scene::~Scene()
{

}

void Pg::Editor::Window::Scene::Initialize()
{
	auto& image = _widgetCon->CreateWidget<Pg::UI::Widget::Image>(_dataContainer->GetSceneTexture(),
		(float)Pg::Data::GameConstantData::WIDTH, (float)Pg::Data::GameConstantData::HEIGHT);
	_imageWidth = image.GetWidth();
	_imageHeight = image.GetHeight();

	std::unique_ptr<Pg::Editor::Event> gizmoType = std::make_unique<Pg::Editor::Event>();
	gizmoType->AddEvent(Pg::Editor::eEventType::_GIZMOTYPE, [&](void* data) { _uiManager->SetGizmoType(data); });
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

	DataSet(_uiManager->GetWindowPosX(), _uiManager->GetWindowPosY(), _uiManager->GetWindowTitleBarHeight());
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

void Pg::Editor::Window::Scene::DataSet(float x, float y, float titleBarHeight)
{
	//바운더리
	float left = x;
	float right = x + *_imageWidth;
	float top = y + titleBarHeight;
	float bottom = y + titleBarHeight + *_imageHeight;

	//Client 기준 X & Y
	float uiMousePosX = _uiManager->GetMousePosX();
	float uiMousePosY = _uiManager->GetMousePosY();

	if (left <= uiMousePosX &&
		right >= uiMousePosX &&
		top <= uiMousePosY &&
		bottom >= uiMousePosY)
	{

		//실제로 씬 윈도우 범위 안에 있는 것.
		float tRealPixelX = uiMousePosX - left;
		float tRealPixelY = uiMousePosY - top;

		float widthRatio = tRealPixelX / *_imageWidth;
		float heightRatio = tRealPixelY / *_imageHeight;

		_mousePos->Invoke(eEventType::_MOUSEPOSX, static_cast<void*>(&widthRatio));
		_mousePos->Invoke(eEventType::_MOUSEPOSY, static_cast<void*>(&heightRatio));
	}
}