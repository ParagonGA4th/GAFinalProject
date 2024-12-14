#include "Image.h"
#include "imgui.h"
#include <algorithm>

Pg::UI::Widget::Image::Image(void* image, float width, float height)
	:_image(image), 
	_orgWidth(width), _orgHeight(height),
	_width(0.f), _height(0.f)
{

}

void Pg::UI::Widget::Image::Update()
{
	///// �ӽ� ����
	//ImVec2 windowSize = ImGui::GetWindowSize();
	//
	//// �̹����� ���� ���
	//float aspectRatio = _orgWidth / _orgHeight;
	//
	//// ���� â�� ���� ���
	//float windowAspectRatio = windowSize.x / windowSize.y;
	//
	//// �̹����� ���ο� ũ�� ���
	//if (windowAspectRatio > aspectRatio) {
	//	// ���� â�� ���� ������ �� ū ���
	//	_width = windowSize.x;
	//	_height = windowSize.x / aspectRatio;
	//}
	//else {
	//	// ���� â�� ���� ������ �� ū ���
	//	_width = windowSize.y * aspectRatio;
	//	_height = windowSize.y;
	//}
	//ImGui::Image(_image, ImVec2(_width, _height));

	// Aspect ������ ����.
	// IMGUI ������ ������ �ޱ�
	ImVec2 windowSize = ImGui::GetWindowSize();

	// ���� ������ ������ �ޱ�
	ImVec2 imageSize(_orgWidth, _orgHeight);

	// ������ �����ϴ� �����ϸ� ���� ����
	float aspectRatio = std::min(windowSize.x / imageSize.x, windowSize.y / imageSize.y);

	// ���� �̹��� ������ ����
	ImVec2 finalImageSize(imageSize.x * aspectRatio, imageSize.y * aspectRatio);
	_width = finalImageSize.x;
	_height = finalImageSize.y;

	// �̹��� ���.
	ImGui::Image(_image, finalImageSize);
}

float* Pg::UI::Widget::Image::GetWidth()
{
	return &_width;
}
float* Pg::UI::Widget::Image::GetHeight()
{
	return &_height;
}

std::string Pg::UI::Widget::Image::GetWidgetLabel()
{
	return "Image";
}

