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
	int count = 0;
	for (auto widget : _widgets)
	{
		if (index != 0)
		{
			delete widget;
		}
		else
		{
			if (index == count)
				delete widget;
			else
				count++;
		}
	}

	_widgets.erase(_widgets.begin() + index, _widgets.end());
}

void Pg::UI::WidgetContainer::ClearColumnWidget()
{
	_columnWidgets.clear();
}

void Pg::UI::WidgetContainer::ClearCollapsWidget()
{
	_collapsWidgets.clear();
}

void Pg::UI::WidgetContainer::ClearTreeNodeWidget(int index)
{
	_treeNodeWidgets.erase(_treeNodeWidgets.begin(), _treeNodeWidgets.begin() + index);
}

void Pg::UI::WidgetContainer::SameLine(bool isSameLine, int lineCount)
{
	_isSameLine = isSameLine;
	_lineCount = lineCount;
}

void Pg::UI::WidgetContainer::ClearTempWidgets()
{
	//for (auto widget : _tempWidgets)
	//{
	//	delete widget;
	//}
	_tempWidgets.clear();
}
