#pragma once
#include "GameObject.h"
#include "Light.h"

namespace Pg::Data
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(GameObject* obj);

		bool operator>(const DirectionalLight& rhs) const {
			return this->_radiance > rhs._radiance;
		}

		//VISITABLE
		PGFLOAT3 _color;
		float _radiance;
		PGFLOAT3 _direction;
		//ENDVISITABLE

		float tTempPadding; // ¾²¸é ¾ÈµÊ.
	};

}


