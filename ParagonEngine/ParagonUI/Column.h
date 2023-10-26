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
		Column(std::vector<Pg::UI::IWidget*> widgets) :_widgets(widgets) {}

		virtual void Update() override
		{
			ImGui::Columns(static_cast<int>(_size), "Test", false);

			int counter = 0;

			for (auto& it : _widgets)
			{
					//if (widths[counter] != -1.f)
					//	ImGui::SetColumnWidth(counter, widths[counter]);

					it->Update();
					ImGui::NextColumn();
				//++counter;

				//if (counter == _Size)
				//	counter = 0;
			}
			ImGui::Columns(1);
		}

	private:
		std::vector<Pg::UI::IWidget*> _widgets;
	};
}

