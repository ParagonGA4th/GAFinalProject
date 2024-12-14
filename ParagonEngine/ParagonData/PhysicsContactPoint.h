#pragma once
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class PhysicsContactPoint
	{
	public:
		//���� �����̽����� Contact ���� ��ġ.
		Pg::Math::PGFLOAT3 _point;

		//Contact�� �븻 ��.
		Pg::Math::PGFLOAT3 _normal;

		//�󸶳� ������ �ִ���! (������ ��վ��ٴ� ���)
		float _separation;
	};
}


