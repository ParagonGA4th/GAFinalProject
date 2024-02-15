#pragma once
#include "IWidget.h"
#include "imgui.h"

#include <vector>

namespace Pg::UI::Widget::Layout
{
	template <size_t _size>
	class Column : public Pg::UI::IWidget
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="id">Column Id(Name)</param>
		/// <param name="widgets">Columns Widgets</param>
		/// <param name="wideColumn">first or second (0/1)</param>
		Column(std::string id, std::vector<Pg::UI::IWidget*> widgets, float* widgetWidth, int wideColumn = 1) 
			:_widgets(widgets), _id(id), _widgetWidth(widgetWidth), _wideColumn(wideColumn) {}

		virtual void Update() override
		{
			int columnCount = static_cast<int>(_size);
			ImGui::Columns(columnCount, _id.c_str(), false);

			int counter = 0;

			for (auto& it : _widgets)
			{
				float orgColSize = *_widgetWidth / (columnCount + 1);

				if (_wideColumn == 1) ImGui::SetColumnWidth(0, orgColSize);
				else ImGui::SetColumnWidth(0, orgColSize * 2);

				//if (_wideColumn == 1) ImGui::SetColumnWidth(_wideColumn, orgColSize * columnCount);
				//else ImGui::SetColumnWidth(_wideColumn, orgColSize);

				if (counter % 2 == _wideColumn)
				{
					ImGui::PushItemWidth(-1);
				}

				it->Update();
				ImGui::NextColumn();
				++counter;

			}
			ImGui::Columns(1);
		}

	private:
		std::vector<Pg::UI::IWidget*> _widgets;
		std::string _id;
		int _wideColumn;
		float* _widgetWidth;
	};
}

