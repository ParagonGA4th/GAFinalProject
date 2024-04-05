#include "Button.h"
#include "imgui.h"

Pg::UI::Widget::Button::Button(std::string label, float width, float height, bool& btnClick)
	:_btnName(label), _width(width), _height(height),
	_isButtonClick(btnClick)
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

std::string Pg::UI::Widget::Button::GetWidgetLabel()
{
	return _btnName;
}
