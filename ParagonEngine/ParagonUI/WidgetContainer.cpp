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

void Pg::UI::WidgetContainer::ClearColumnWidget()
{
	_columnWidgets.clear();
}
