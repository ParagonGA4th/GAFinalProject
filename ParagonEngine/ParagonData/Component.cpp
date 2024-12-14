#include "Component.h"

namespace Pg::Data
{
	Component::Component(GameObject* obj) :
		_object(obj),
		_isActive(true)
	{

	}

	Component::~Component()
	{
		OnDestroy();
	}

	void Component::SetActive(bool active)
	{
		if (active != _isActive)
		{
			_isActive = active;
		}
	}

	bool Component::GetActive()
	{
		return _isActive;
	}

}
