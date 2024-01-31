#include "Image.h"
#include "imgui.h"

Pg::UI::Widget::Image::Image(void* image, float width, float height)
	:_image(image), _width(width), _height(height)
{

}

void Pg::UI::Widget::Image::Update()
{
	/// 임시 설정
	ImVec2 windowSize = ImGui::GetWindowSize();

	// 이미지의 원래 크기
	float originalWidth = _width;
	float originalHeight = _height;

	// 이미지의 비율 계산
	float aspectRatio = originalWidth / originalHeight;

	// 현재 창의 비율 계산
	float windowAspectRatio = windowSize.x / windowSize.y;

	// 이미지의 새로운 크기 계산
	float newWidth, newHeight;
	if (windowAspectRatio > aspectRatio) {
		// 현재 창의 가로 비율이 더 큰 경우
		newWidth = windowSize.x;
		newHeight = windowSize.x / aspectRatio;
	}
	else {
		// 현재 창의 세로 비율이 더 큰 경우
		newWidth = windowSize.y * aspectRatio;
		newHeight = windowSize.y;
	}

	ImGui::Image(_image, ImVec2(newWidth, newHeight));
}


