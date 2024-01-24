#include "Button.h"
#include "imgui.h"

bool Pg::UI::Widget::Button::_isButtonClick = false;

Pg::UI::Widget::Button::Button(std::string label, float x, float y)
	:_btnName(label), _x(x), _y(y)
{
}

void Pg::UI::Widget::Button::Update()
{
	if(ImGui::Button(_btnName.c_str(), ImVec2(_x, _y)))
	{
		_isButtonClick = true;
	}

	_isButtonClick = false;
}