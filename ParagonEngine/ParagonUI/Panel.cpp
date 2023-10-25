#include "Panel.h"
#include "ILayout.h"
#include "ImGuiManager.h"

Pg::UI::Panel::Panel(std::string panelName)
	:_panelName(panelName)
{
	//_currentLayout = new ILayout();
	//_layouts.emplace_back(_currentLayout);
}

Pg::UI::Panel::~Panel()
{

}

std::string Pg::UI::Panel::GetPanelName()
{
	return _panelName;
}

void Pg::UI::Panel::Update()
{
	Pg::UI::Manager::ImGuiManager::PanelBegin(_panelName);
	for (auto& layout : _layouts)
	{
		layout->Update();
	}
	Pg::UI::Manager::ImGuiManager::PanelEnd();
}

void Pg::UI::Panel::CreateLayout(std::string layoutName, Pg::UI::Enum::eLayoutType layoutType)
{
	switch (layoutType)
	{
	case Pg::UI::Enum::eLayoutType::Collapsing:
		//_currentLayout = new Pg::UI::ILayout("");
		_layouts.emplace_back(_currentLayout);
		break;

	case Pg::UI::Enum::eLayoutType::Menu:
		//_currentLayout = new Pg::UI::ILayout("");
		_layouts.emplace_back(_currentLayout);
		break;
	}
}

void Pg::UI::Panel::SwitchingLayout(std::string panelName)
{
	for (auto& layout : _layouts)
	{
		//if (layout->GetPanelName() == panelName)
		//{
		//	_currentLayout = layout;
		//	break;
		//}
	}
}

Pg::UI::ILayout* Pg::UI::Panel::GetCurrentLayout()
{
	return _currentLayout;
}


