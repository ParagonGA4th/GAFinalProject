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
		/// <param name="widthSet">width setting</param>
		/// <param name="wideColumn">first or second (0/1)</param>
		Column(std::string id, std::vector<Pg::UI::IWidget*> widgets, bool widthSet = true, int wideColumn = 1) 
			:_widgets(widgets), _id(id), _isWidthSet(widthSet), _wideColumn(wideColumn) {}

		virtual void Update() override
		{
			ImGui::Columns(static_cast<int>(_size), _id.c_str(), false);

			int counter = 0;

			for (auto& it : _widgets)
			{
				if (_isWidthSet)
				{
					if (_wideColumn == 1)
					{
						ImGui::SetColumnWidth(0, 110.f);
						ImGui::SetColumnWidth(_wideColumn, 290.f);
					}
					else
						ImGui::SetColumnWidth(_wideColumn, 210.f);

					if (counter % 2 == _wideColumn)
					{
						ImGui::PushItemWidth(-1);
					}
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
		bool _isWidthSet;
		int _wideColumn;
	};
}

