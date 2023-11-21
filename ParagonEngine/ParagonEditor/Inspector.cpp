#include "Inspector.h"
#include "DataContainer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
 #include <singleton-cpp/singleton.h>

// Object Name
// Object Tag
// Active
// Transform
// Component

#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/InputFloat3.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	cons = new Pg::UI::WidgetContainer();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{
	Pg::Data::GameObject* obj = nullptr;

	if (_dataContainer->GetCurrentScene() != NULL)
	{
		for (auto vobj : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			if (vobj->GetName() == "New Object") obj = vobj;
		}

		_objName = obj->GetName();
		_isObjActive = obj->GetActive();
	}

	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", _objName);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", _objTag);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	cons->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", _isObjActive);

	cons->CreateWidget<Pg::UI::Widget::Layout::Column<2>>(cons->GetColumnWidgets());


	cons->ClearColumnWidget();
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Position");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Position", _position);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Rotation");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Rotation", _rotation);
	cons->CreateColumnsWidget<Pg::UI::Widget::Text>("Scale");
	cons->CreateColumnsWidget<Pg::UI::Widget::InputFloat3>("Scale", _scale);

	cons->CreateCollapsWidget<Pg::UI::Widget::Layout::Column<2>>(cons->GetColumnWidgets());

	cons->CreateWidget<Pg::UI::Widget::Layout::Collaps>("Transform", cons->GetCollapsWidgets());	
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	cons->Update();
	_uiManager->WindowEnd();

	if (_dataContainer->GetSave())
	{
		for (auto vobj : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			if (vobj->GetName() == "New Object")
			{
				vobj->SetName(_objName);
				vobj->SetActive(_isObjActive);
			}
		}
	}
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
