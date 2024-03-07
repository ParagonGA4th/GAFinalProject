#pragma once

#include "../ParagonData/Script.h"

#include <auto_register/factory.h>

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}

using Pg::Data::GameObject;
using Pg::Data::Script;

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}
using Pg::API::Input::PgInput;
