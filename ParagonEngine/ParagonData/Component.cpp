#include "Component.h"

namespace Pg::Data
{
	Component::Component(GameObject* obj) :
		_object(obj)
	{

	}

	Component::~Component()
	{
		OnDestroy();
	}
}
