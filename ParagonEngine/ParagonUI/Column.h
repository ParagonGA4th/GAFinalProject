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
		Column(std::string id, std::vector<Pg::UI::IWidget*> widgets, bool widthSet = true) :_widgets(widgets), _id(id), _widthSet(widthSet) {}

		virtual void Update() override
		{
			ImGui::Columns(static_cast<int>(_size), _id.c_str(), false);

			int counter = 0;

			for (auto& it : _widgets)
			{
				if (_widthSet)
				{
					ImGui::SetColumnWidth(0, 130.f);

					if (counter % 2 == 1)
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
		bool _widthSet;
	};
}

