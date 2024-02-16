#include "Button.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"
#include "../ParagonUtil/Log.h"

#include "GameObject.h"

namespace Pg::Data
{
	class GameObject;

	Button::Button(GameObject* obj)
		:RendererBase2D(obj),
		_onClickEvent(),
		_isPressed(false)
	{
		obj->AddComponent<ImageRenderer>();
		obj->AddComponent<TextRenderer>();
	}

	void Button::Update()
	{
		PGFLOAT3 worldPos = _object->_transform._position;
		_box2dInfo.LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		_box2dInfo.RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
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

		PG_TRACE("Button Click!");
	}
}
