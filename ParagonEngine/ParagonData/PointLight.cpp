#include "PointLight.h"

namespace Pg::Data
{

	PointLight::PointLight(GameObject* obj)
		: Light(obj),
		_attenuation(PGFLOAT3{0.f, 0.f, 0.f}),
		_range(0.f)
	{

	}

	void Pg::Data::PointLight::SetAttenuation(PGFLOAT3 attenuation)
	{
		_attenuation = attenuation;
	}

	Pg::Math::PGFLOAT3 Pg::Data::PointLight::GetAttenuation()
	{
		return _attenuation;
	}

	void PointLight::SetRange(float d)
	{
		_range = d;
	}

	float PointLight::GetRange()
	{
		return _range;
	}


}

