#pragma once

namespace Pg::Data::Enums
{
	enum class eLightType : unsigned int
	{
		_NONE = 0,
		_DIRECTIONALLIGHT = 1,
		_POINTLIGHT = 2,
		_SPOTLIGHT = 3
	};
}