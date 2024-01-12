#include "InspectorHelper.h"

#include "../ParagonData/GameObject.h"

#include "../ParagonUI/WidgetContainer.h"

#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/InputFloat3.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

#include <visit_struct/visit_struct_intrusive.hpp>



Pg::Editor::Window::InspectorHelper::InspectorHelper()
{
	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();
}

Pg::Editor::Window::InspectorHelper::~InspectorHelper()
{

}

void Pg::Editor::Window::InspectorHelper::Initialize()
{
	// 기본 정보
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", _objName);

	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", _objTag);
	
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", _isActive);

	_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Column<2>>(_widgetCon->GetColumnWidgets());
}

void Pg::Editor::Window::InspectorHelper::SetData(Pg::Data::GameObject* object)
{
	_object = object;

	_widgetCon->ClearWidget(1);
	ComponentUI();	
	//TransformComponent();
}

void Pg::Editor::Window::InspectorHelper::Update()
{
	_widgetCon->Update();

	// objct에서 get()을 통해 가져오는 값은 수정이 불가함
	//_object->SetName(_objName);
	//_object->SetActive(_isActive);
	//_object->SetTag(_objTag);
}

void Pg::Editor::Window::InspectorHelper::TransformComponent()
{
	// Transform Component
	_widgetCon->ClearColumnWidget();
	_widgetCon->ClearCollapsWidget();

	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Position");
	_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Position", &_object->_transform._position);

	//_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Rotation");
	//_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Rotation", &_object->_transform._rotation);
	
	//_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("Scale");
	//_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Scale", &_object->_transform._scale);

	_widgetCon->CreateCollapsWidget<Pg::UI::Widget::Layout::Column<2>>(_widgetCon->GetColumnWidgets());

	_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Collaps>("Transform", _widgetCon->GetCollapsWidgets());
}

void Pg::Editor::Window::InspectorHelper::ComponentUI()
{
	for (auto component : _object->GetComponentList())
	{
		std::string componentType;
		
		if (typeid(Pg::Data::Transform).name() == componentType)
		{

		}

		componentType = componentType.substr(componentType.rfind("::") + 2);


		_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Column<2>>(_widgetCon->GetColumnWidgets());

		_widgetCon->ClearColumnWidget();
		_widgetCon->ClearCollapsWidget();

		// 변수의 개수 만큼
		_widgetCon->CreateColumnsWidget<Pg::UI::Widget::Text>("변수명");
		//_widgetCon->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Position", &_object->_transform._position);



		_widgetCon->CreateCollapsWidget<Pg::UI::Widget::Layout::Column<2>>(_widgetCon->GetColumnWidgets());
		_widgetCon->CreateWidget<Pg::UI::Widget::Layout::Collaps>(componentType, _widgetCon->GetCollapsWidgets());
	}
}
