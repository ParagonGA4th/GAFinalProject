#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
	
namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;

	class Light : public Component
	{
	public:
		Light(GameObject* obj);
	};

}


