#include "Collaps.h"
#include "imgui.h"

Pg::UI::Layout::Collaps::Collaps(std::string LayoutName, std::vector<Pg::UI::IWidget*> widgets, bool* isShow)
	: _layoutName(LayoutName), _widgets(widgets), _isShow(isShow)
{

}

void Pg::UI::Layout::Collaps::Update()
{
	if (ImGui::CollapsingHeader(_layoutName.c_str(), _isShow, ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& widget : _widgets)
		{
			widget->Update();
		}
	}
}

std::string Pg::UI::Layout::Collaps::GetWidgetLabel()
{
	return _layoutName;
}

