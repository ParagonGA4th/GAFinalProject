#include "Hierarchy.h"
#include "DataContainer.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Selectable.h"

#include "../ParagonData/Scene.h"


#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Hierarchy::Hierarchy()
	:_winName("Hierarchy"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	cons = new Pg::UI::WidgetContainer();
}

Pg::Editor::Window::Hierarchy::~Hierarchy()
{

}

void Pg::Editor::Window::Hierarchy::Initialize()
{
	cons->CreateWidget<Pg::UI::Widget::Selectable>(_objNames);
}

void Pg::Editor::Window::Hierarchy::Update()
{
	_uiManager->WindowBegin(_winName);
	DataSet();
	cons->Update();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Hierarchy::Finalize()
{

}

void Pg::Editor::Window::Hierarchy::SetShow(bool show)
{ 
	_isShow = show;
}

bool Pg::Editor::Window::Hierarchy::GetShow()
{
	return _isShow;
}

void Pg::Editor::Window::Hierarchy::DataSet()
{
	if (_dataContainer->GetCurrentScene() != NULL)
	{
		_objNames.clear();
		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			_objNames.emplace_back(i->GetName());
		}
	}
}
