#include "DontDestroy.h"
#include "DataContainer.h"

#include "../ParagonData/Scene.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Selectable.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::DontDestroy::DontDestroy()
	:_winName("DontDestory"), _isShow(true)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();
}

Pg::Editor::Window::DontDestroy::~DontDestroy()
{
}

void Pg::Editor::Window::DontDestroy::Initialize()
{
	_widgetCon->CreateWidget<Pg::UI::Widget::Selectable>(&_dontDestroyObjList, _selectedName);
}

void Pg::Editor::Window::DontDestroy::Update()
{
	_uiManager->WindowBegin(_winName);
	_uiManager->BeginDisable(_isDisable);

	DataSet();
	_widgetCon->Update();

	if (_isDisable) _uiManager->EndDisable();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::DontDestroy::Finalize()
{

}

void Pg::Editor::Window::DontDestroy::DataSet()
{
	for (auto& obj : Pg::Data::Scene::_dontDestroyOnList)
	{
		_dontDestroyObjList.emplace_back(obj->GetName());
	}
}

void Pg::Editor::Window::DontDestroy::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::DontDestroy::GetShow()
{
	return _isShow;
}

std::string Pg::Editor::Window::DontDestroy::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::DontDestroy::SetDisable(bool disable)
{
	_isDisable = disable;
}