#include "InspectorUIManager.h"
#include "InspectorDataManager.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/Light.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonScript/Script.h"


#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Button.h"
#include "../ParagonUI/Combo.h"
#include "../ParagonUI/DragFloat.h"
#include "../ParagonUI/DragFloat3.h"

#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

Pg::Editor::Window::InspectorUIManager::InspectorUIManager()
{
	_defaultUI = std::make_unique<Pg::UI::WidgetContainer>();
	_changedUI = std::make_unique<Pg::UI::WidgetContainer>();
}

Pg::Editor::Window::InspectorUIManager::~InspectorUIManager()
{

}

void Pg::Editor::Window::InspectorUIManager::Initialize(InspectorDataManager* manager)
{
	_dataManager = manager;

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", &_objName);

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", &_objTag);

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", &_isActive);

	_defaultUI->CreateWidget<Pg::UI::Layout::Column<2>>("BasicInfo", _defaultUI->GetColumnWidgets());

	_defaultUI->ClearColumnWidget();

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
	SetData();
	_defaultUI->Update();
	UpdateData();
	ChangedUI();
}

void Pg::Editor::Window::InspectorUIManager::ChangedUI()
{
	_changedUI->ClearWidget();

	for (auto& object : _dataManager->_objectData)
	{
		if (object.first != _dataManager->_object->GetName()) continue;

		for (auto& component : object.second)
		{
			/// component Name
			std::string comName;
			if (component.first.find("::") == std::string::npos) comName = component.first.substr(component.first.rfind("class") + 6);
			else comName = component.first.substr(component.first.rfind("::") + 2);

			if (_componentExistence.find(component.first) == _componentExistence.end())
				_componentExistence.insert({ component.first , new bool(true) });
			else
				*_componentExistence.at(component.first) = true;


			_changedUI->ClearColumnWidget();
			_changedUI->ClearCollapsWidget();


			/// component Data
			for (auto& [valName, typeInfo, val] : component.second)
			{
				valName = valName.substr(valName.rfind("_") + 1);
				_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>(valName);

				if (typeInfo == typeid(bool).name())
					_changedUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>(valName, static_cast<bool*>(val));

				if (typeInfo == typeid(std::string).name())
				{
					if (comName.find("StaticMeshRenderer") != std::string::npos)
					{
						// assetManager의 namelist 를 받아와서 출력,
						//_changedUI->CreateColumnsWidget<Pg::UI::Widget::Combo>(valName, static_cast<std::string*>(val));

					}
					else
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::InputText>(valName, static_cast<std::string*>(val));
				}

				if (typeInfo == typeid(float).name())
					_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat>(valName, static_cast<float*>(val));

				if (typeInfo == typeid(Pg::Math::PGFLOAT3).name())
				{
					if (valName.find("scale") != std::string::npos)
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGFLOAT3*>(val), 0.01f);
					else
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGFLOAT3*>(val));
				}

				if (typeInfo == typeid(Pg::Math::PGFLOAT4).name())
					_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGFLOAT4*>(val));

				if (typeInfo == typeid(Pg::Math::PGQuaternion).name())
					_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGQuaternion*>(val));
			}

			_changedUI->CreateCollapsWidget<Pg::UI::Layout::Column<2>>(comName, _changedUI->GetColumnWidgets());
			
			_changedUI->CreateWidget<Pg::UI::Layout::Collaps>
				(comName, _changedUI->GetCollapsWidgets(), _componentExistence.at(component.first));

			if (comName.find("StaticMeshRenderer") != std::string::npos)
			{
				auto& btn = _changedUI->CreateWidget<Pg::UI::Widget::Button>("Refresh", 115.f, 30.f);
				_isRefresh = btn.GetBtnClick();
			}
		}
	}

	_changedUI->Update();
}

void Pg::Editor::Window::InspectorUIManager::SetData()
{
	_objName = _dataManager->_object->GetName();
	_objTag = _dataManager->_object->GetTag();
	_isActive = _dataManager->_object->GetActive();
}

void Pg::Editor::Window::InspectorUIManager::UpdateData()
{
	if (*_isClick) _dataManager->AddComponent(_componentList.at(*_componentIndex));

	for (auto& [name, val] : _componentExistence)
	{
		if (*val == false)
		{
			_dataManager->RemoveComponent(name);

			auto iter = _componentExistence.find(name);
			delete iter->second;
			_componentExistence.erase(iter);
			break;
		}
	}

	_dataManager->_object->SetName(_objName);
	_dataManager->_object->SetTag(_objTag);
	_dataManager->_object->SetActive(_isActive);

	if (_isRefresh != nullptr && *_isRefresh)
	{
		_dataManager->ModifiedObject();
	}
}

