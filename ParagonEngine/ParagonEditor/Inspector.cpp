#include "Inspector.h"
#include "InspectorHelper.h"
#include "Event.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/Light.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/Script.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Button.h"
#include "../ParagonUI/Combo.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();
	_insHelper = std::make_unique<Pg::Editor::Window::InspectorHelper>();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{	
	_insHelper->Initialize();

	std::unique_ptr<Pg::Editor::Event> changeObjectData = std::make_unique<Pg::Editor::Event>();
	changeObjectData->AddEvent(Pg::Editor::eEventType::_OBJECTDATA, [&](void* data) { SetData(data); });

	_componentList.emplace_back(typeid(Pg::Data::Transform).name());
	_componentList.emplace_back(typeid(Pg::Data::Camera).name());
	_componentList.emplace_back(typeid(Pg::Data::Light).name());
	_componentList.emplace_back(typeid(Pg::Data::StaticMeshRenderer).name());
	_componentList.emplace_back(typeid(Pg::Data::Script).name());

	auto& combo = _widgetCon->CreateColumnsWidget<Pg::UI::Widget::Combo>("##Add Component", _componentList);
	_componentIndex = combo.GetSelectedIndex();
	
	auto& btn = _widgetCon->CreateColumnsWidget<Pg::UI::Widget::Button>("Add Component", 115.f, 30.f);
	_isClick = btn.GetBtnClick();

	_widgetCon->CreateWidget<Pg::UI::Layout::Column<2>>("AddComponent", _widgetCon->GetColumnWidgets(), 0);
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	_insHelper->Update();
	_widgetCon->Update();
	AddComponent();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Inspector::Finalize()
{

}

void Pg::Editor::Window::Inspector::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Inspector::GetShow()
{
	return _isShow;
}

void Pg::Editor::Window::Inspector::SetData(void* data)
{
	if (data != nullptr)
	{
		_selectGameObject = static_cast<Pg::Data::GameObject*>(data);
		_insHelper->SetData(_selectGameObject);
	}
}

void Pg::Editor::Window::Inspector::AddComponent()
{
	if (*_isClick)
	{
		if (_selectGameObject != nullptr)
		{
			_selectGameObject->AddComponent(_componentList.at(*_componentIndex));
			_insHelper->SetData(_selectGameObject);
		}
	}
}
