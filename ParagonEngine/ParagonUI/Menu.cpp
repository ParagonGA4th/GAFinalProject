#include "Menu.h"
#include "imgui.h"

Pg::UI::Widget::Menu::Menu(std::string menuName, std::vector<Pg::UI::IWidget*> widgets)
	:_menuName(menuName), _widgets(widgets)
{

}

void Pg::UI::Widget::Menu::Update()
{
	if(ImGui::BeginMenu(_menuName.c_str()))
	{
		for (auto& i : _widgets)
		{
			i->Update();
		}
		ImGui::EndMenu();
	}
}
