#include "Script.h"

namespace Pg::script
{
	Script::Script(Pg::Data::GameObject* obj): 
		Component(obj)
	{

	}

	Script::~Script()
	{
		OnDestroy();
	}
}