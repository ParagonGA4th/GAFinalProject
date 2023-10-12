#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
	
namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;

	class Light : public Component
	{
	public:
		Light(GameObject* obj);

		void SetIntensity(float d);
		float GetIntensity();

		void SetLightColor(PGFLOAT4 color);
		void SetLightColor(float r, float g, float b, float a);
		PGFLOAT4 GetLightColor();

		void SetAmbient(PGFLOAT4 rad);
		PGFLOAT4 GetAmbient();

		void SetSpecular(PGFLOAT4 rad);
		PGFLOAT4 GetSpecular();

		void SetDiffuse(PGFLOAT4 rad);
		PGFLOAT4 GetDiffuse();

	private:
		PGFLOAT4 _ambient;
		PGFLOAT4 _specular;
		PGFLOAT4 _diffuse;

		PGFLOAT4 _color;
		float _intensity;
	};

}


