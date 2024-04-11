#include "Text.h"
#include "imgui.h"

Pg::UI::Widget::Text::Text(std::string text)
	:_context(text)
{
}

void Pg::UI::Widget::Text::Update()
{
	ImGui::Text(_context.c_str());
}

std::string Pg::UI::Widget::Text::GetWidgetLabel()
{
	return "Text";
}
