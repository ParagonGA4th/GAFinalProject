#include "Slider.h"
#include "GameObject.h"
#include "ImageRenderer.h"
namespace Pg::Data
{
	Slider::Slider(GameObject* owner) :
		Component(owner),
		_onValueEvent(),
		_value(0.0f)
	{
		if (owner->GetComponent<ImageRenderer>())
		{
			assert(false);
		}

		_imageRenderer = owner->AddComponent<ImageRenderer>();
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);
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

	}

	float Slider::GetImageHeight()
	{

	}

}