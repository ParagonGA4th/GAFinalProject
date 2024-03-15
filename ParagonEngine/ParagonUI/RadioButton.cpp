#include "RadioButton.h"
#include "imgui.h"

Pg::UI::Widget::RadioButton::RadioButton(std::string label, bool& active)
	:_btnName(label), _isButtonClick(active)
{
}

void Pg::UI::Widget::RadioButton::Update()
{
	if (ImGui::RadioButton(_btnName.c_str(), _isButtonClick))
	{
		_isButtonClick = true;
	}
}
