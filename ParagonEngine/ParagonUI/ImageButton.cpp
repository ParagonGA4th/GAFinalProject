#include "ImageButton.h"
#include "imgui.h"

bool Pg::UI::Widget::ImageButton::_isButtonClick = false;

Pg::UI::Widget::ImageButton::ImageButton(void* texture, float width, float height)
	:_texture(texture), _width(width), _height(height)
{

}

void Pg::UI::Widget::ImageButton::Update()
{
	if (ImGui::ImageButton(_texture, ImVec2(_width, _height)))
	{
		_isButtonClick = true;
	}
	_isButtonClick = false;
}

std::string Pg::UI::Widget::ImageButton::GetWidgetLabel()
{
	return "ImageButton";
}
