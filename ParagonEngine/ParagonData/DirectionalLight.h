#pragma once
#include "GameObject.h"
#include "Light.h"

namespace Pg::Data
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(GameObject* obj);
		void SetDirection(const PGFLOAT3& direction);
		PGFLOAT3& GetDirection();

	private:
		PGFLOAT3 _direction;
		float _radiance;
	};

}


