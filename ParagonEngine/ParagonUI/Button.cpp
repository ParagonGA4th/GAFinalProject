#include "Button.h"
#include "imgui.h"

Pg::UI::Widget::Button::Button(std::string label, float width, float height)
	:_btnName(label), _width(width), _height(height),
	_isButtonClick(false)
{
}

void Pg::UI::Widget::Button::Update()
{
	if(ImGui::Button(_btnName.c_str(), ImVec2(_width, _height)))
	{
		_isButtonClick = true;
	}
	else
	{
		_isButtonClick = false;
	}
}

bool* Pg::UI::Widget::Button::GetBtnClick()
{
	return &_isButtonClick;
}
