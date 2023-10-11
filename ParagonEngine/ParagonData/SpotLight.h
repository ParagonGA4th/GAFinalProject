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
		PGFLOAT3 GetAttenuation();
		void SetRange(float d);
		float GetRange();

	private:
		float _range;
		PGFLOAT3 _attenuation;
	};
}

