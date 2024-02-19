#include "Slider.h"
#include "GameObject.h"

namespace Pg::Data
{
	Slider::Slider(GameObject* owner) :
		Component(owner),
		_onValueEvent(),
		_value(0.0f)
	{

	}

	void Slider::Update()
	{
		
	}

	void Slider::SetValue(float val)
	{

	}

	float Slider::GetValue()
	{
		return _value;
	}

}