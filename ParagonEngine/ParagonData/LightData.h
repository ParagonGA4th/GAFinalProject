#pragma once
#include"../ParagonMath/PgMath.h"

namespace Pg::Data::Enums
{
	// �ʿ����� ���� ���� ����......
	class LightType;
}

namespace Pg::Data
{
	using namespace Pg::Math;

	struct DirectionalLightData
	{
		PGFLOAT4 _ambient;
		PGFLOAT4 _diffuse;
		PGFLOAT4 _specular;

		PGFLOAT4 _color;
		float _intensity;
	};

	struct PointLightData
	{
		PGFLOAT4 _ambient;
		PGFLOAT4 _diffuse;
		PGFLOAT4 _specular;

		PGFLOAT4 _color;
		float _intensity;

		float _range;
		PGFLOAT3 _attenuation;
	};

	struct SpotLightData
	{
		PGFLOAT4 _ambient;
		PGFLOAT4 _diffuse;
		PGFLOAT4 _specular;

		PGFLOAT4 _color;
		float _intensity;

		float _range;
		PGFLOAT3 _attenuation;
	};

}