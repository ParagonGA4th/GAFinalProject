#include "MenuBar.h"
#include "imgui.h"

Pg::UI::Widget::MenuBar::MenuBar(std::map<std::string, std::map<std::string, bool*>> menubars)
	:_menubars(menubars)
{
}

void Pg::UI::Widget::MenuBar::Update()
{
	if (ImGui::BeginMenuBar())
	{
		for (auto& menu : _menubars)
		{
			size_t pos;
			std::stoi(menu.first, &pos);

			if (ImGui::BeginMenu(menu.first.substr(pos).c_str()))
			{
				for (auto& item : menu.second)
				{
					std::stoi(item.first, &pos);
					std::string itemName = item.first.substr(pos);

					if (itemName.find("Line") != std::string::npos) ImGui::Separator();
					else ImGui::MenuItem(itemName.c_str(), NULL, item.second);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
}

std::string Pg::UI::Widget::MenuBar::GetWidgetLabel()
{
	return "MenuBar";
}
