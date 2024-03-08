#include "Slider.h"
#include "GameObject.h"
#include "Button.h"
#include "ImageRenderer.h"
//#include "../ParagonEngine/"

namespace Pg::Data
{
	Slider::Slider(GameObject* owner) :
		Component(owner),
		_onValueEvent(),
		_value(0.0f),
		_isClick(false)
	{
		if (owner->GetComponent<ImageRenderer>())
		{
			assert(false);
		}

		_imageRenderer = owner->AddComponent<ImageRenderer>();
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);
	}

	void Slider::Start()
	{
		//버튼이 자식 객체로써 존재한다.
		GameObject* buttonObject = new GameObject("sliderBtn");
		buttonObject->AddComponent<Button>();
		
		GameObject* handleObject = new GameObject("handle");
		handleObject->AddComponent<ImageRenderer>();

		//_min = this->_transform
	}

	void Slider::Update()
	{
		
	}

	void Slider::SetValue(float val)
	{
		_value = val;
	}

	float Slider::GetValue()
	{
		return _value;
	}

	void Slider::SetValueEvent(std::function<void(float)> event)
	{
		_onValueEvent = event;
	}

	std::function<void(float)> Slider::GetvalueEvent()
	{
		return _onValueEvent;
	}

	void Slider::SetImageSize(float width, float height)
	{
		SetImageWidth(width);
		SetImageHeight(height);
	}

	void Slider::SetImagePath(const std::string path)
	{
		_imageRenderer->SetImagePath(path);
	}

	void Slider::SetImageWidth(float width)
	{
		*_imageWidth = width;
	}

	void Slider::SetImageHeight(float height)
	{
		*_imageHeight = height;
	}

	float Slider::GetImageWidth()
	{
		return *_imageWidth;
	}

	float Slider::GetImageHeight()
	{
		return *_imageHeight;
	}
}