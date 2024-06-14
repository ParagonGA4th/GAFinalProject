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
		//PGFLOAT3 _direction; -> 이건 무조건 Quaternion의 Transform에서 들고 와야 한다.
		float _range;
		PGFLOAT3 _attenuation;
		//ENDVISITABLE

		float tTempPadding; //쓰면 안됨.
	};
}

