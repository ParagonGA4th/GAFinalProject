#pragma once

namespace Pg::Data::Enums
{
	enum class eLightType : unsigned int
	{
		NONE = 0,
		DIRECTIONALLIGHT = 1,
		POINTLIGHT = 2,
		SPOTLIGHT = 3
	};
}