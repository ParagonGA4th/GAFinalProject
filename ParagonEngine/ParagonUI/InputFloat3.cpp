#include "InputFloat3.h"
#include "imgui.h"

Pg::UI::Widget::InputFloat3::InputFloat3(std::string label, float input[3])
	: _label("##"), _inputfloat()
{
	_label.append(label);
	_inputPtr = input;
	memcpy(_inputfloat, input, sizeof(float) * 3);
} 

void Pg::UI::Widget::InputFloat3::Update()
{
	memcpy(_inputfloat, _inputPtr, sizeof(float) * 3);

	if (ImGui::InputFloat3(_label.c_str(), _inputfloat))
	{
		*_inputPtr = _inputfloat[0];
		*(_inputPtr + 1) = _inputfloat[1];
		*(_inputPtr + 2)= _inputfloat[2];
	}
}


