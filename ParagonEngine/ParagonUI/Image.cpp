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
	///// 임시 설정
	//ImVec2 windowSize = ImGui::GetWindowSize();
	//
	//// 이미지의 비율 계산
	//float aspectRatio = _orgWidth / _orgHeight;
	//
	//// 현재 창의 비율 계산
	//float windowAspectRatio = windowSize.x / windowSize.y;
	//
	//// 이미지의 새로운 크기 계산
	//if (windowAspectRatio > aspectRatio) {
	//	// 현재 창의 가로 비율이 더 큰 경우
	//	_width = windowSize.x;
	//	_height = windowSize.x / aspectRatio;
	//}
	//else {
	//	// 현재 창의 세로 비율이 더 큰 경우
	//	_width = windowSize.y * aspectRatio;
	//	_height = windowSize.y;
	//}
	//ImGui::Image(_image, ImVec2(_width, _height));

	// Aspect 유지를 위해.
	// IMGUI 윈도우 사이즈 받기
	ImVec2 windowSize = ImGui::GetWindowSize();

	// 원래 윈도우 사이즈 받기
	ImVec2 imageSize(_orgWidth, _orgHeight);

	// 비율을 보존하는 스케일링 팩터 연산
	float aspectRatio = std::min(windowSize.x / imageSize.x, windowSize.y / imageSize.y);

	// 최종 이미지 사이즈 연산
	ImVec2 finalImageSize(imageSize.x * aspectRatio, imageSize.y * aspectRatio);
	_width = finalImageSize.x;
	_height = finalImageSize.y;

	// 이미지 출력.
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

