#include "Button.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"
#include "GameConstantData.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/InputSystem.h"

#include "GameObject.h"
#include <cassert>

namespace Pg::Data
{
	class GameObject;

	Button::Button(GameObject* owner)
		:Component(owner),
		_onClickEvent(),
		_isPressed(false)
	{
		FACTORY_INIT;

		//input 적용
		auto& _tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		_inputSystem = &_tInputSystem;	

		
		//j->AddComponent<TextRenderer>();
	}

	void Button::GrabManagedObjects()
	{
		if (!_object->GetComponent<ImageRenderer>())
		{
			assert(false && "Button의 ImageRenderer Component가 없습니다");
		}

		//이미지 기본으로 들어있어야 함.
		_imageRenderer = _object->GetComponent<ImageRenderer>();
		assert(_imageRenderer != nullptr);
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);
	}

	void Button::Update()
	{
		PGFLOAT3 worldPos = _object->_transform._position;

		_box2dInfo.LT = { worldPos.x - (*_imageWidth / 2.0f), worldPos.y - (*_imageHeight / 2.f) };
		_box2dInfo.RB = { worldPos.x + (*_imageWidth / 2.0f), worldPos.y + (*_imageHeight / 2.f) };
		_box2dInfo.color = { 1.0f,0.0f,0.0f,1.0f };

		// gainput을 사용하여 비율 변환한 Click 처리
		const float mouseX = _inputSystem->GetMouseX();
		const float mouseY = _inputSystem->GetMouseY();

		const float objectX = _object->_transform._position.x;
		const float objectY = _object->_transform._position.y;

		// 화면 비율에 따라 마우스 위치 변환
		const float scaledMouseX = mouseX * GameConstantData::WIDTH;
		const float scaledMouseY = mouseY * GameConstantData::HEIGHT;

		if (scaledMouseX > (objectX - GetImageWidth() / 2.0f) &&
			scaledMouseX <  (objectX + GetImageWidth() / 2.0f) &&
			scaledMouseY >(objectY - GetImageHeight() / 2.0f) &&
			scaledMouseY < (objectY + GetImageHeight() / 2.0f))
		{
			Hover();

			if (_inputSystem->GetKeyDown(API::Input::MouseLeft))
			{
				ClickDown();
				PG_TRACE("ClickDown!");
			}
			else if (_inputSystem->GetKeyUp(API::Input::MouseLeft))
			{
				ClickUp();
				PG_TRACE("ClickUp!");
			}
		}
		else
		{
			NotHover();
		}
	}

	void Button::SetOnClickDownEvent(std::function<void()> onClickEvent)
	{
		_onClickEvent = onClickEvent;
	}

	std::function<void()> Button::GetOnClickDownEvent() const
	{
		return _onClickEvent;
	}

	void Button::SetHover(std::function<void()> hover)
	{
		_hover = hover;
	}

	std::function<void()> Button::GetHover() const
	{
		return _hover;
	}

	void Button::SetNotHover(std::function<void()> notHover)
	{
		_notHover = notHover;
	}

	std::function<void()> Button::GetNotHover() const
	{
		return _notHover;
	}

	void Button::SetOnClickUpEvent(std::function<void()> onClickEvent)
	{
		_onClickUpEvent = onClickEvent;
	}

	std::function<void()> Button::GetOnClickUpEvent() const
	{
		return _onClickUpEvent;
	}

	void Button::ClickDown()
	{
		if (_onClickEvent)
		{
			_onClickEvent();
		}

		_isPressed = !_isPressed;
	}

	void Button::Hover()
	{
		//PG_TRACE("ClickStay!!");
		if (_hover)
		{
			_hover();
		}
	}

	void Button::NotHover()
	{
		if (_notHover)
		{
			_notHover();
		}

	}

	void Button::ClickUp()
	{
		if (_onClickUpEvent)
		{
			_onClickUpEvent();
		}

		_isPressed = !_isPressed;
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

	ImageRenderer* Button::GetImageRenderer()
	{
		return _imageRenderer;
	}
}
