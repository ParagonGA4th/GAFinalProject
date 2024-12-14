#include "Slider.h"
#include "GameObject.h"
#include "Button.h"
#include "Handle.h"
#include "Scene.h"
#include "GameConstantData.h"
#include "ImageRenderer.h"

///슬라이더는 기본적으로 Input이 존재해야 한다.
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonUtil/Log.h"
#include <algorithm>
#include <cassert>

namespace Pg::Data
{
	Slider::Slider(GameObject* owner) :
		Component(owner),
		_onValueEvent(),
		_value(0.0f),
		_isClick(false)
	{
		FACTORY_INIT;

		//input 적용
		auto& _tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		_inputSystem = &_tInputSystem;
	}

	void Slider::Internal_EngineAwake()
	{
		
	}

	void Slider::Start()
	{
		
		
	}

	void Slider::Update()
	{
		if (handleObj == nullptr)
		{
			FindRequiredObject();
		}

		if (_inputSystem->GetKeyDown(API::Input::MouseLeft))
		{
			if (_inputSystem->GetMouseX() >= _minWidth && _inputSystem->GetMouseX() <= _maxWidth &&
				_inputSystem->GetMouseY() >= _minHeight && _inputSystem->GetMouseY() <= _maxHeight)
			{
				_isClick = true;
			}
		}

		if (_isClick)
		{
			//PG_TRACE("SLIDE~~");
			
			float newPosition = _inputSystem->GetMouseX();

			newPosition = std::clamp(newPosition, _minWidth, _maxWidth);

			handleObj->_object->_transform._position.x = newPosition * GameConstantData::WIDTH;

			_value = (newPosition - _minWidth) / (_maxWidth - _minWidth);

			PG_WARN(_value);

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

	void Slider::FindRequiredObject()
	{
		if (!_object->GetComponent<ImageRenderer>())
		{
			assert(false && "Slider의 ImageRenderer Component가 없습니다");
		}

		_imageRenderer = _object->GetComponent<ImageRenderer>();
		_imageRenderer->_sortingLayer = 2;
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);

		//Scene에서 설정했던 Handle객체를 찾는다.
		handleObj = _object->GetScene()->FindSingleComponentInScene<Handle>();

		//for (auto iter : handleObj)
		//{
		//	Pg::Data::GameObject* tGO = iter->_object;
		//	auto tHandle = tGO->GetComponent<Handle>();
		//	if (tHandle != nullptr)
		//	{
		//		_handle = tHandle;
		//		break;
		//	}
		//}

		//버튼이 자식 객체로써 존재한다.
		//GameObject* buttonObject = new GameObject("sliderBtn");
		//buttonObject->AddComponent<Button>();

		assert(handleObj != nullptr && "이 시점에서 무조건 Handle 있어야 함");

		//핸들의 위치 한정.
		handleObj->_object->_transform._position = this->_object->_transform._position;

		{
			float tPixelMin = _object->_transform._position.x - (_imageRenderer->_width / 2);
			float tPixelMax = _object->_transform._position.x + (_imageRenderer->_width / 2);
			_minWidth = tPixelMin / GameConstantData::WIDTH;
			_maxWidth = tPixelMax / GameConstantData::WIDTH;
		}
		{
			float tPixelMin = _object->_transform._position.y - (_imageRenderer->_height / 2);
			float tPixelMax = _object->_transform._position.y + (_imageRenderer->_height / 2);
			_minHeight = tPixelMin / GameConstantData::HEIGHT;
			_maxHeight = tPixelMax / GameConstantData::HEIGHT;
		}
	}

}