#pragma once
#include "Light.h"


namespace Pg::Data
{
	using namespace Pg::Math;

	class PointLight : public Light
	{
	public:
		PointLight(GameObject* obj);

		bool operator>(const PointLight& rhs) const {
			return this->_radiance > rhs._radiance;
		}

		//VISITABLE
		PGFLOAT3 _color;
		float _radiance;
		float _range;
		PGFLOAT3 _attenuation;
		//ENDVISITABLE
	};
	
}


