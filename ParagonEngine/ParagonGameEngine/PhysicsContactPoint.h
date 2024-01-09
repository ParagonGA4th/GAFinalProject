#pragma once
#include "../ParagonMath/PgMath.h"

namespace Pg::Engine
{
	class PhysicsContactPoint
	{
	public:
		//월드 스페이스에서 Contact 지점 위치.
		Pg::Math::PGFLOAT3 _point;

		//Contact의 노말 값.
		Pg::Math::PGFLOAT3 _normal;

		//얼마나 떨어져 있는지! (음수면 꿰뚫었다는 얘기)
		float _separation;
	};
}


