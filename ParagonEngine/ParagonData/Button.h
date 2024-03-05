#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"


#include <functional>

/// <summary>
/// 버튼을 위한 클래스. 
/// 23.10.10
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class ImageRenderer;

	class Button : public Component
	{
	public:
		Button(GameObject* obj);

	public:
		virtual void Update() override;

		void Click();

		void SetOnClickEvent(std::function<void()> onClickEvent);
		std::function<void()> GetOnClickEvent() const;

		void SetImagePath(const std::string& path);
		void SetImageSize(float width, float height);
		
		void SetImageWidth(float width);
		float GetImageWidth();
		
		void SetImageHeight(float height);
		float GetImageHeight();

	public:
		//이미지의 사이즈
		float* _imageWidth;
		float* _imageHeight;

	public:
		//Debug System용 사용
		Box2DInfo _box2dInfo;

	private:
		ImageRenderer* _imageRenderer = nullptr;

		std::function<void()> _onClickEvent;

		bool _isPressed;	//버튼 누른 여부 체크.
	};
}


