#pragma once
#include "IWidget.h"
#include "imgui.h"

#include <vector>

namespace Pg::UI::Layout
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
		Column(std::string id, std::vector<Pg::UI::IWidget*> widgets, int wideColumn = 1) 
			:_widgets(widgets), _id(id),  _wideColumn(wideColumn) {}

		virtual void Update() override
		{
			int columnCount = static_cast<int>(_size);
			ImGui::Columns(columnCount, _id.c_str(), false);

			int counter = 0;

			for (auto& it : _widgets)
			{
				float orgColSize = ImGui::GetWindowSize().x / (columnCount + 1);

				if (_wideColumn == 1) ImGui::SetColumnWidth(0, orgColSize);
				else ImGui::SetColumnWidth(0, orgColSize * 2);

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

		virtual std::string GetWidgetLabel() override;

	private:
		std::vector<Pg::UI::IWidget*> _widgets;
		std::string _id;
		int _wideColumn;
	};

	template <size_t _size>
	std::string Pg::UI::Layout::Column<_size>::GetWidgetLabel()
	{
		return _id;
	}
}

