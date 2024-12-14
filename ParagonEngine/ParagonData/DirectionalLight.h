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
		//PGFLOAT3 _direction; -> 이건 Transform Quaternion에서 매번 가져와야 한다.
		//ENDVISITABLE

		float tTempPadding; // 쓰면 안됨.
	};

}


