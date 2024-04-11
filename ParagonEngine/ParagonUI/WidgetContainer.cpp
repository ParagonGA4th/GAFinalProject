#include "WidgetContainer.h"
#include "imgui.h"

void Pg::UI::WidgetContainer::Update()
{
	int count = 0;
	for (auto& i : _widgets)
	{
		i->Update();
		if (_isSameLine)
		{
			if (count < _lineCount) ImGui::SameLine();
			if (count > _lineCount)	ImGui::SameLine();

			count++;
		}
	}

	if (_isSameLine) _isSameLine = false;
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

std::vector<Pg::UI::IWidget*> Pg::UI::WidgetContainer::GetChildWindowWidgets()
{
	return _childWindowWidgets;
}

void Pg::UI::WidgetContainer::SetTempWidgets(std::vector<Pg::UI::IWidget*> tempWidget)
{
	for (auto& widget : tempWidget)
	{
		_tempWidgets.emplace_back(widget);
	}
}

std::vector<Pg::UI::IWidget*> Pg::UI::WidgetContainer::GetTempWidgets()
{
	return _tempWidgets;
}

void Pg::UI::WidgetContainer::ClearWidget(int index)
{
	_widgets.erase(_widgets.begin() + index, _widgets.end());
}

void Pg::UI::WidgetContainer::ClearColumnWidget()
{
	_columnWidgets.clear();
}

void Pg::UI::WidgetContainer::ClearCollapsWidget(std::string label)
{
	_collapsWidgets.erase(std::remove_if(_collapsWidgets.begin(), _collapsWidgets.end(),
		[&](Pg::UI::IWidget* widget)
		{
			if (widget->GetWidgetLabel() != label) return true;
		}), _collapsWidgets.end());
}

void Pg::UI::WidgetContainer::ClearTreeNodeWidget(int index)
{
	if (index == -1) _treeNodeWidgets.clear();
	else _treeNodeWidgets.erase(_treeNodeWidgets.begin() + index);
}

void Pg::UI::WidgetContainer::SameLine(bool isSameLine, int lineCount)
{
	_isSameLine = isSameLine;
	_lineCount = lineCount;
}

void Pg::UI::WidgetContainer::ClearTempWidgets()
{	
	_tempWidgets.clear();
}
