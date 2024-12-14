#include "InputInt.h"
#include "imgui.h"

Pg::UI::Widget::InputInt::InputInt(std::string label, int* input)
	: _label("##"), _inputFloat(input)
{
	_label.append(label);
}

void Pg::UI::Widget::InputInt::Update()
{
	ImGui::InputInt(_label.c_str(), _inputFloat);
}

std::string Pg::UI::Widget::InputInt::GetWidgetLabel()
{
	return _label;
}

