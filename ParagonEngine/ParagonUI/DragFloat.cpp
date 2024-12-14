#include "DragFloat.h"
#include "imgui.h"

Pg::UI::Widget::DragFloat::DragFloat(std::string label, float* input)
	: _label("##"), _inputFloat(input)
{
	_label.append(label);
}

void Pg::UI::Widget::DragFloat::Update()
{
	ImGui::DragFloat(_label.c_str(), _inputFloat, 0.005f);
}

std::string Pg::UI::Widget::DragFloat::GetWidgetLabel()
{
	return _label;
}

