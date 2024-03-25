#include "Script.h"

namespace Pg::DataScript
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