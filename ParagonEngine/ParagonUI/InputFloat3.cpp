#include "InputFloat3.h"
#include "imgui.h"

Pg::UI::Widget::InputFloat3::InputFloat3(std::string label, Pg::Math::PGFLOAT3* input)
	: _label("##"), _inputPGFloat(input)
{
	_label.append(label);

	_inputFloat[0] = _inputPGFloat->x;
	_inputFloat[1] = _inputPGFloat->y;
	_inputFloat[2] = _inputPGFloat->z;
} 

void Pg::UI::Widget::InputFloat3::Update()
{
	_inputFloat[0] = _inputPGFloat->x;
	_inputFloat[1] = _inputPGFloat->y;
	_inputFloat[2] = _inputPGFloat->z;

	ImGui::InputFloat3(_label.c_str(), _inputFloat);

	_inputPGFloat->x = _inputFloat[0];
	_inputPGFloat->y = _inputFloat[1];
	_inputPGFloat->z = _inputFloat[2];
}


