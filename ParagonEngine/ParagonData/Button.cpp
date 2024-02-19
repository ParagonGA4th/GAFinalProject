#include "Button.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"
#include "../ParagonUtil/Log.h"

#include "GameObject.h"

namespace Pg::Data
{
	class GameObject;

	Button::Button(GameObject* obj)
		:Component(obj),
		_onClickEvent(),
		_isPressed(false)
	{
		
		if (obj->GetComponent<ImageRenderer>())
		{
			assert(false);
		}
		
		
		_imageRenderer = obj->AddComponent<ImageRenderer>();
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);
		
		//j->AddComponent<TextRenderer>();
	}

	void Button::Update()
	{
		PGFLOAT3 worldPos = _object->_transform._position;

		_box2dInfo.LT = { worldPos.x - (*_imageWidth / 2.0f), worldPos.y - (*_imageHeight / 2.f) };
		_box2dInfo.RB = { worldPos.x + (*_imageWidth / 2.0f), worldPos.y + (*_imageHeight / 2.f) };
		_box2dInfo.color = { 1.0f,0.0f,0.0f,1.0f };
	}

	void Button::SetOnClickEvent(std::function<void()> onClickEvent)
	{
		_onClickEvent = onClickEvent;
	}

	std::function<void()> Button::GetOnClickEvent() const
	{
		return _onClickEvent;
	}

	void Button::Click()
	{
		if (_onClickEvent)
		{
			_onClickEvent();
		}

		_isPressed = !_isPressed;

		//PG_TRACE("Button Click!");
	}

	void Button::SetImagePath(const std::string& path)
	{
		_imageRenderer->SetImagePath(path);
	}

	void Button::SetImageSize(float width, float height)
	{
		SetImageWidth(width);
		SetImageHeight(height);
	}

	void Button::SetImageWidth(float width)
	{
		*_imageWidth = width;
	}

	void Button::SetImageHeight(float height)
	{
		*_imageHeight = height;
	}

	float Button::GetImageWidth()
	{
		return *_imageWidth;
	}

	float Button::GetImageHeight()
	{
		return *_imageHeight;
	}

}
