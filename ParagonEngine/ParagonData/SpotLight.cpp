#include "SpotLight.h"


namespace Pg::Data
{
	SpotLight::SpotLight(GameObject* obj)
		: Light(obj),
		_attenuation(PGFLOAT3{0.f, 0.f, 0.f}),
		_range(0.f)
	{

	}

	void SpotLight::SetAttenuation(PGFLOAT3 attenuation)
	{
		_attenuation = attenuation;
	}

	Pg::Math::PGFLOAT3 SpotLight::GetAttenuation()
	{
		return _attenuation;
	}

	void SpotLight::SetRange(float d)
	{
		_range = d;
	}

	float SpotLight::GetRange()
	{
		return _range;
	}
}

