#include "Script.h"

namespace Pg::Data
{
	Script::Script(GameObject* obj): 
		Component(obj)
	{

	}

	Script::~Script()
	{
		OnDestroy();
	}
}