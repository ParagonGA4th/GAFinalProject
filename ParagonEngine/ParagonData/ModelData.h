#pragma once
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::Data
{
	struct Texture
	{
		std::string name;
		std::string path;
	};	

	struct Shader
	{
		std::string name;
		std::string path;
	};

	struct Material
	{
		std::string name;
		std::string path;

		// phong
		Pg::Math::PGFLOAT4 diffuse;
		Pg::Math::PGFLOAT4 ambient;
		Pg::Math::PGFLOAT4 specular;

		// pbr
		Pg::Math::PGFLOAT4 albedo;
		Pg::Math::PGFLOAT4 roughness;
		float metalic;
	};
	
	struct Model
	{
		std::string name;
		std::string path;
	};

}
