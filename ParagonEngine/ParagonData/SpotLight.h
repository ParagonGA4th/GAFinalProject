#pragma once
#include "Light.h"

namespace Pg::Data
{
	class SpotLight : public Light
	{
	public:
		SpotLight(GameObject* obj);

	public:
		void SetAttenuation(PGFLOAT3 attenuation);
		virtual PGFLOAT3 GetAttenuation();
		void SetRange(float d);
		virtual float GetRange();
		void SetDirection(const PGFLOAT3& direction);
		PGFLOAT3& GetDirection();

	private:
		float _range;
		PGFLOAT3 _attenuation;
		PGFLOAT3 _direction;
	};
}

