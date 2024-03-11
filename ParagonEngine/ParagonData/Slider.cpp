#include "Slider.h"
#include "GameObject.h"
#include "Button.h"
#include "Handle.h"
#include "ImageRenderer.h"

///슬라이더는 기본적으로 Input이 존재해야 한다.
#include "../ParagonUtil/InputSystem.h"
#include <algorithm>

namespace Pg::Data
{
	Slider::Slider(GameObject* owner) :
		Component(owner),
		_onValueEvent(),
		_value(0.0f),
		_isClick(false)
	{
		//input 적용
		auto& _tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		_inputSystem = &_tInputSystem;

		if (owner->GetComponent<ImageRenderer>())
		{
			assert(false);
		}

		_imageRenderer = owner->AddComponent<ImageRenderer>();
		_imageRenderer->_sortingLayer = 2;
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);

		//Scene에서 설정했던 Handle객체를 찾는다.
		auto handleObj = owner->GetComponent<Transform>()->GetChildren();

		for (auto iter : handleObj)
		{

		}
	}

	void Slider::Start()
	{
		//버튼이 자식 객체로써 존재한다.
		//GameObject* buttonObject = new GameObject("sliderBtn");
		//buttonObject->AddComponent<Button>();

		_min = _object->_transform._position.x - (_imageRenderer->_width / 2);
		_max = _object->_transform._position.x + _imageRenderer->_width / 2;
	}

	void Slider::Update()
	{
		if (_inputSystem->GetKeyDown(API::Input::MouseLeft))
		{
			if (_inputSystem->GetMouseDX() >= _min && _inputSystem->GetMouseDX() <= _max)
			{
				_isClick = true;
			}
		}

		if (_isClick)
		{
			float newPosition = _inputSystem->GetMouseDX();

			newPosition = std::clamp(newPosition, _min, _max);

			_handle->_object->_transform._position.x = newPosition;

			_value = (newPosition - _min) / (_max - _min);

			if (_onValueEvent)
			{
				_onValueEvent(_value);
			}

			if (_inputSystem->GetKeyUp(API::Input::MouseLeft))
			{
				_isClick = false;
			}
		}
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