#include "WidgetContainer.h"

void Pg::UI::WidgetContainer::Update()
{
	for (auto& i : _widgets)
	{
		i->Update();
	}
}

std::vector<Pg::UI::IWidget*> Pg::UI::WidgetContainer::GetColumnWidgets()
{
	return _columnWidgets;
}

std::vector<Pg::UI::IWidget*> Pg::UI::WidgetContainer::GetCollapsWidgets()
{
	return _collapsWidgets;
}

std::vector<Pg::UI::IWidget*> Pg::UI::WidgetContainer::GetTreeNodeWidgets()
{
	return _treeNodeWidgets;
}

void Pg::UI::WidgetContainer::ClearColumnWidget()
{
	_columnWidgets.clear();
}

void Pg::UI::WidgetContainer::ClearCollapsWidget()
{
	_collapsWidgets.clear();
}

void Pg::UI::WidgetContainer::ClearTreeNodeWidget()
{
	_treeNodeWidgets.clear();	
}
