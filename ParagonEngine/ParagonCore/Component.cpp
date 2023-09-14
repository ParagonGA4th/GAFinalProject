#include "Component.h"

namespace Pg::Core
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
