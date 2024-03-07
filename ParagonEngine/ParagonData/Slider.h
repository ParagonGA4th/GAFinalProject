#pragma once
#include "Component.h"

#include <functional>
/// <summary>
/// 변지상의 Slider 컴포넌트.
/// Slider의 변경값에 따라 콜백되는 이벤트가 있어야 한다.
/// </summary>
namespace Pg::Data
{
	class GameObject;
	class ImageRenderer;

	class Slider : public Component
	{
	public:
		Slider(GameObject* owner);

		virtual void Start() override;
		virtual void Update() override;

		void SetValueEvent(std::function<void(float)> event);
		std::function<void(float)> GetvalueEvent();

		void SetImagePath(const std::string path);

		void SetImageSize(float width, float height);
		void SetImageWidth(float width);
		void SetImageHeight(float height);

		float GetImageWidth();
		float GetImageHeight();

		void SetValue(float val);
		float GetValue();


	private:
		//슬라이더의 값이 변경될 때 발생하는 이벤트.
		std::function<void(float)> _onValueEvent;
		float _value;

		ImageRenderer* _imageRenderer = nullptr;

	public:
		//이미지의 사이즈
		float* _imageWidth;
		float* _imageHeight;

		//슬라이더의 최대 최소값
		int _min = 0;
		int _max = 100;

		bool _isClick;
	};
}
