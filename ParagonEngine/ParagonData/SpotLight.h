#pragma once
#include "Light.h"

namespace Pg::Data
{
	class SpotLight : public Light
	{
	public:
		SpotLight(GameObject* obj);

		bool operator>(const SpotLight& rhs) const {
			return this->_radiance > rhs._radiance;
		}

		//VISITABLE
		PGFLOAT3 _color;
		float _radiance;
		PGFLOAT3 _direction;
		float _range;
		PGFLOAT3 _attenuation;
		//ENDVISITABLE

		float tTempPadding; //¾²¸é ¾ÈµÊ.
	};
}

