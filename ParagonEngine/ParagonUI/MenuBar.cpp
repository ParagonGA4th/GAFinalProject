#include "MenuBar.h"
#include "imgui.h"

Pg::UI::Widget::MenuBar::MenuBar(std::unordered_map<std::string, std::unordered_map<std::string, bool*>> manubar)
	:_manubars(manubar)
{
}

void Pg::UI::Widget::MenuBar::Update()
{
	if (ImGui::BeginMenuBar())
	{
		for (auto& menu : _manubars)
		{
			if (ImGui::BeginMenu(menu.first.c_str()))
			{
				for (auto& item : menu.second)
				{
					if(item.first == "Line") ImGui::Separator();
					else ImGui::MenuItem(item.first.c_str(), NULL, item.second);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
}