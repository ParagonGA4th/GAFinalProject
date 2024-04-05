#include "ChildWindow.h"
#include "imgui.h"

Pg::UI::Layout::ChildWindow::ChildWindow(std::string layoutName, Pg::Math::PGFLOAT2 size, std::vector<Pg::UI::IWidget*> widgets)
	:_layoutName(layoutName), _size(size), _widgets(widgets)
{

}

void Pg::UI::Layout::ChildWindow::Update()
{
	ImGui::BeginChild(_layoutName.c_str(), {_size.x, _size.y}, true);

	for (auto& it : _widgets)
	{
		it->Update();
	}

	ImGui::EndChild();
}

std::string Pg::UI::Layout::ChildWindow::GetWidgetLabel()
{
	return _layoutName;
}

