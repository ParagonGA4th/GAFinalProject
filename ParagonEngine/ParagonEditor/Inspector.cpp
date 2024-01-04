#include "Inspector.h"
#include "Event.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"

#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/InputFloat3.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

#include <singleton-cpp/singleton.h>


Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	cons = new Pg::UI::WidgetContainer();

	_changeObjectData = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{	
	// 기본 정보
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", _objName);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", _objTag);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	cons->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", _isObjActive);

	cons->CreateWidget<Pg::UI::Widget::Layout::Column<2>>(cons->GetColumnWidgets());

	// Transform Component
	cons->ClearColumnWidget();
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Position");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Position", _position);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Rotation");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Rotation", _rotation);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Scale");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Scale", _scale);

	cons->CreateCollapsWidget<Pg::UI::Widget::Layout::Column<2>>(cons->GetColumnWidgets());

	cons->CreateWidget<Pg::UI::Widget::Layout::Collaps>("Transform", cons->GetCollapsWidgets());

	_changeObjectData->AddEvent(Pg::Editor::eEventType::ChangeObjectData, [&](void* data) { DataSet(data); });
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	DataUpate();
	cons->Update();
	_uiManager->WindowEnd();

	//if (_dataContainer->GetSave())
	//{
	//	for (auto vobj : _dataContainer->GetCurrentScene()->GetObjectList())
	//	{
	//		if (vobj->GetName() == "New Object")
	//		{
	//			vobj->SetName(_objName);
	//			vobj->SetActive(_isObjActive);
	//		}
	//	}
	//}
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

void Pg::Editor::Window::Inspector::DataSet(void* data)
{
	if (data != nullptr)
	{
		_gameObject = data;
		Pg::Data::GameObject* obj = static_cast<Pg::Data::GameObject*>(_gameObject);

		if (obj != NULL)
		{
			_objName = obj->GetName();
			_isObjActive = obj->GetActive();

			_position[0] = obj->_transform._position.x;
			_position[1] = obj->_transform._position.y;
			_position[2] = obj->_transform._position.z;

			_rotation[0] = obj->_transform._rotation.x;
			_rotation[1] = obj->_transform._rotation.y;
			_rotation[2] = obj->_transform._rotation.z;

			_scale[0] = obj->_transform._scale.x;
			_scale[1] = obj->_transform._scale.y;
			_scale[2] = obj->_transform._scale.z;
		}
	}
}

void Pg::Editor::Window::Inspector::DataUpate()
{
	if (_gameObject != nullptr)
	{
		Pg::Data::GameObject* obj = static_cast<Pg::Data::GameObject*>(_gameObject);
		
		obj->SetActive(_isObjActive);
		obj->SetName(_objName);

		obj->_transform._position = { _position[0], _position[1], _position[2] };
		obj->_transform._rotation = { 1.f, _rotation[0], _rotation[1], _rotation[2] };
		obj->_transform._scale = { _scale[0], _scale[1], _scale[2] };
	}
}
