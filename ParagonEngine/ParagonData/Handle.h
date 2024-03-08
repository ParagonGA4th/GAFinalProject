#pragma once
#include "Component.h"

/// <summary>
/// 슬라이더에 붙일 Handle 클래스.
/// 얘는 그냥 Transform이랑 ImageRenderer만 들고 있으면 됨.
/// 자식 객체로 넣을 예정.
/// 2023.03.08
/// </summary>
namespace Pg::Data
{
	class GameObject;
	class ImageRenderer;

	class Handle : public Component
	{
	public:
		Handle(GameObject* owner);

		void SetImagePath(const std::string& path);
		void SetImageSize(float width, float height);

		void SetImageWidth(float width);
		float GetImageWidth();

		void SetImageHeight(float height);
		float GetImageHeight();

	private:
		ImageRenderer* _imageRenderer = nullptr;

		float* _imageWidth;
		float* _imageHeight;
	};
}

