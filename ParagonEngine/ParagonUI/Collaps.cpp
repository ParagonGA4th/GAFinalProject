#include "Collaps.h"
#include "imgui.h"

Pg::UI::Widget::Layout::Collaps::Collaps(std::string LayoutName, std::vector<Pg::UI::IWidget*> widgets)
	: _layoutName(LayoutName), _widgets(widgets)
{

}

void Pg::UI::Widget::Layout::Collaps::Update()
{
	if (ImGui::CollapsingHeader(_layoutName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& widget : _widgets)
		{
			widget->Update();
		}
	}
}


