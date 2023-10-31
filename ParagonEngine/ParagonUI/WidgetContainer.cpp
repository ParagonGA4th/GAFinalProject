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
