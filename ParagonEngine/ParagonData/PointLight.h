#pragma once
#include "Light.h"


namespace Pg::Data
{
	using namespace Pg::Math;

	class PointLight : public Light
	{
	public:
		PointLight(GameObject* obj);

	public:
		void SetAttenuation(PGFLOAT3 attenuation);
		PGFLOAT3 GetAttenuation();
		void SetRange(float d);
		float GetRange();


	private:
		PGFLOAT3 _attenuation;
		float _range;
	};
	
}


