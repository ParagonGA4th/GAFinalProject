#include "InspectorUIManager.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/Light.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonScript/Script.h"


#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Button.h"
#include "../ParagonUI/Combo.h"

Pg::Editor::Window::InspectorUIManager::InspectorUIManager()
{
	_defaultUI = std::make_unique<Pg::UI::WidgetContainer>();
	_changedUI = std::make_unique<Pg::UI::WidgetContainer>();
}

Pg::Editor::Window::InspectorUIManager::~InspectorUIManager()
{

}

void Pg::Editor::Window::InspectorUIManager::Initialize()
{
	//_componentList.emplace_back(typeid(Pg::Data::Camera).name());
	//_componentList.emplace_back(typeid(Pg::Data::Transform).name());
	//_componentList.emplace_back(typeid(Pg::Data::AudioSource).name());
	_componentList.emplace_back(typeid(Pg::Data::StaticMeshRenderer).name());
	//_componentList.emplace_back(typeid(Pg::Data::Light).name());
	//_componentList.emplace_back(typeid(Pg::DataScript::Script).name()); 

	auto& combo = _defaultUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##Add Component", _componentList);
	_componentIndex = combo.GetSelectedIndex();

	auto& btn = _defaultUI->CreateColumnsWidget<Pg::UI::Widget::Button>("Add Component", 115.f, 30.f);
	_isClick = btn.GetBtnClick();

	_defaultUI->CreateWidget<Pg::UI::Layout::Column<2>>("AddComponent", _defaultUI->GetColumnWidgets(), 0);
}

void Pg::Editor::Window::InspectorUIManager::Update()
{
	_defaultUI->Update();
}
