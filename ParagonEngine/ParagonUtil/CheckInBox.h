#pragma once
#include "UtilDLLExporter.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Util
{
	class CheckInBox
	{
	public:
		PARAGON_UTIL_DLL CheckInBox();

		PARAGON_UTIL_DLL static bool IsIn3DBox(
			Pg::Math::PGFLOAT3 col1Transform, float width1, float height1, float depth1,
			Pg::Math::PGFLOAT3 col2Transform,float width2, float height2, float depth2);
		
		PARAGON_UTIL_DLL static bool IsIn2DBox(
			Pg::Math::PGFLOAT3 col1Transform, float width1, float height1,
			Pg::Math::PGFLOAT3 col2Transform, float width2, float height2);

	};
}

