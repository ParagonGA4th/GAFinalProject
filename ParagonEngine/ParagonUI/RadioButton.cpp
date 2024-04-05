#include "RadioButton.h"
#include "imgui.h"

Pg::UI::Widget::RadioButton::RadioButton(std::string label, std::string& selectRadio)
	:_btnName(label), _selectRadio(selectRadio)
{
}

void Pg::UI::Widget::RadioButton::Update()
{
	if (ImGui::RadioButton(_btnName.c_str(), _btnName == _selectRadio))
	{
		_selectRadio = _btnName;
	}
}

std::string Pg::UI::Widget::RadioButton::GetWidgetLabel()
{
	return _btnName;
}
