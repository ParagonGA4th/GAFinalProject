#include "Image.h"
#include "imgui.h"

Pg::UI::Widget::Image::Image(void* image, float width, float height)
	:_image(image), _width(width), _height(height)
{

}

void Pg::UI::Widget::Image::Update()
{
	ImGui::Image(_image, ImVec2(_width, _height));
}


