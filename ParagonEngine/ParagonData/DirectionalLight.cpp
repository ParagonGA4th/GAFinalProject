#include "DirectionalLight.h"

namespace Pg::Data
{

	DirectionalLight::DirectionalLight(GameObject* obj)
		: Light(obj)
	{

	}

	void DirectionalLight::SetDirection(const PGFLOAT3& direction)
	{
		_direction = direction;
	}

	Pg::Math::PGFLOAT3& DirectionalLight::GetDirection()
	{
		return _direction;
	}

}