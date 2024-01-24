#include "InputFloat.h"
#include "imgui.h"

Pg::UI::Widget::InputFloat::InputFloat(std::string label, float* input)
	: _label("##"), _inputFloat(input)
{
	_label.append(label);
}

void Pg::UI::Widget::InputFloat::Update()
{
	ImGui::InputFloat(_label.c_str(), _inputFloat);
}


