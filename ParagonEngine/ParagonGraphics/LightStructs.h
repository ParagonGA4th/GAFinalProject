#pragma once

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/LightType.h"

namespace Pg::Data::Structs
{
	using namespace Pg::Math;

	struct Light
	{
		PGFLOAT4 position;

		PGFLOAT4 color;
		PGFLOAT4 ambient;
		PGFLOAT4 diffuse;
		PGFLOAT4 specular;

		Pg::Data::Enums::eLightType type;
		float intensity;
		
		PGFLOAT3 direction;

		float range;
		PGFLOAT3 attenuation;

	};

	__declspec(align(16)) struct DirectionalLight
	{
		PGFLOAT4 position;

		PGFLOAT4 color;

		PGFLOAT4 ambient;
		PGFLOAT4 diffuse;
		PGFLOAT4 specular;

		float intensity;

		PGFLOAT3 direction;
	};

	__declspec(align(16)) struct PointLight
	{
		PGFLOAT4 position;

		PGFLOAT4 color;

		PGFLOAT4 ambient;
		PGFLOAT4 diffuse;
		PGFLOAT4 specular;

		float intensity;

		PGFLOAT3 attenuation;
		float range;
	};

	__declspec(align(16)) struct SpotLight
	{
		PGFLOAT4 position;

		PGFLOAT4 color;

		PGFLOAT4 ambient;
		PGFLOAT4 diffuse;
		PGFLOAT4 specular;

		float intensity;

		PGFLOAT3 direction;

		PGFLOAT3 attenuation;
		float range;
	};

	__declspec(align(16)) struct LightingData
	{
		PGFLOAT3 camPosW;
		unsigned int bufferToRender;
	};
}