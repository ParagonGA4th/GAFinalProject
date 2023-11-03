#pragma once

#include "../ParagonMath/PgMath.h"

namespace Pg::Data::Structs
{
	using namespace Pg::Math;

	struct Light
	{
		PGFLOAT4 color;
		PGFLOAT4 ambient;
		PGFLOAT4 diffuse;
		PGFLOAT4 Specullar;
		float intensity;
	};

	struct DirectionalLight : public Light
	{
		PGFLOAT3 direction;
	};

	struct PointLight : public Light
	{
		PGFLOAT3 attenuation;
		float range;
	};

	struct SpotLight : public Light
	{
		PGFLOAT3 attenuation;
		float range;
	};
}