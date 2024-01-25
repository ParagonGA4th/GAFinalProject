#include "InspectorHelper.h"
#include "DataContainer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include "../ParagonUI/WidgetContainer.h"

#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/InputFloat.h"
#include "../ParagonUI/InputFloat3.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

#include <visit_struct/visit_struct_intrusive.hpp>
#include <singleton-cpp/singleton.h>



Pg::Editor::Window::InspectorHelper::InspectorHelper()
{
	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	auto& tdcon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdcon;
}

Pg::Editor::Window::InspectorHelper::~InspectorHelper()
{

}

void Pg::Editor::Window::InspectorHelper::Initialize()
{
	// 기본 정보
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", &_objName);

	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", &_objTag);
	
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", _isActive);

	_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Column<2>>("BasicInfo", _widgetCon->GetColumnWidgets());
}

void Pg::Editor::Window::InspectorHelper::SetData(Pg::Data::GameObject* object)
{
	_object = object;

	_isActive = _object->GetActive();
	_objName = _object->GetName();
	_objTag = _object->GetTag();

	_widgetCon->ClearWidget(1);
	ComponentUI();	
}

void Pg::Editor::Window::InspectorHelper::Update()
{
	_widgetCon->Update();

	if (_object != nullptr)
	{
		// object의 기본 정보 및 타입 변환 후 정보
		_object->SetActive(_isActive);
		_object->SetName(_objName);
		_object->SetTag(_objTag);

		_object->_transform._rotation.x = _tempFloat3.x;
		_object->_transform._rotation.y = _tempFloat3.y;
		_object->_transform._rotation.z = _tempFloat3.z;
	}
}

void Pg::Editor::Window::InspectorHelper::ComponentUI()
{
	if (_dataContainer->GetScenesData()->empty()) return;

	auto it = _dataContainer->GetScenesData()->find(_dataContainer->GetCurrentScene()->GetSceneName());
	if (it != _dataContainer->GetScenesData()->end())
	{
		for (auto object : it->second)
		{
			if (_object->GetName() == object.first)
			{
				if (_object->GetComponentList().size() != object.second.size())
				{
					_isAddComponent = true;
					object.second.clear();

					for (auto objComs : _object->GetComponentList())
					{
						std::vector<std::tuple<std::string, std::string, void*>> tTempVec;

						objComs.second->OnDeserialize(tTempVec);
						object.second[objComs.first] = tTempVec;
					}
				}

				for (auto component : object.second)
				{
					_widgetCon->ClearColumnWidget();
					_widgetCon->ClearCollapsWidget();

					for (auto& [valName, typeInfo, val] : component.second)
					{
						if (_isAddComponent) valName = valName.substr(valName.rfind("_") + 1);
						_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>(valName);

						if (typeInfo == typeid(std::string).name())
						{
							_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputText>(valName, static_cast<std::string*>(val));
						}						
						
						if (typeInfo == typeid(float).name())
						{
							_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat>(valName, static_cast<float*>(val));
						}

						if (typeInfo == typeid(Pg::Math::PGFLOAT3).name())
						{
							_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>(valName, static_cast<Pg::Math::PGFLOAT3*>(val));
						}
						
						if (typeInfo == typeid(Pg::Math::PGFLOAT4).name())
						{
							_tempFloat4 = static_cast<Pg::Math::PGFLOAT4*>(val);

							_tempFloat3.x = _tempFloat4->x;
							_tempFloat3.y = _tempFloat4->y;
							_tempFloat3.z = _tempFloat4->z;

							_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>(valName, &_tempFloat3);
						}						
						
						if (typeInfo == typeid(Pg::Math::PGQuaternion).name())
						{
							_tempQut = static_cast<Pg::Math::PGQuaternion*>(val);

							_tempFloat3.x = _tempQut->x;
							_tempFloat3.y = _tempQut->y;
							_tempFloat3.z = _tempQut->z;

							_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>(valName, &_tempFloat3);
						}
					}

					_widgetCon->CreateCollapsWidget<Pg::UI::Widget::Layout::Column<2>>(component.first.substr(component.first.rfind("::") + 2), _widgetCon->GetColumnWidgets());
					_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Collaps>(component.first.substr(component.first.rfind("::") + 2), _widgetCon->GetCollapsWidgets());
				}
				_isAddComponent ? false : false;
			}
		}
	}
}
