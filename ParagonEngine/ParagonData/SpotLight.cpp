#include "SpotLight.h"


namespace Pg::Data
{
	SpotLight::SpotLight(GameObject* obj)
		: Light(obj),
		_attenuation(PGFLOAT3{ 0.f, 0.f, 0.f }),
		_range(0.f)
	{

	}
}

