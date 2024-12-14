#pragma once
#include "../ParagonMath/PgMath.h"

//Effect Render Object : ���� ��������� / ������ �� �ֵ��� �������.

namespace Pg::Data
{
	struct VisualEffectRenderObject
	{
	public:
		//IsFaceCamera / Is3DSpace ���� ���� ������ ���� ��.
		//Ex. IsFaceCamera�� Rotation�� �ݿ��� �ȵ�.
		//Ex. Is3DSpace�� False��, Position�� X,Y�� ���̰� .. etc.
		Pg::Math::PGFLOAT3 _position{0,0,0};
		Pg::Math::PGQuaternion _rotation{1,0,0,0};
		Pg::Math::PGFLOAT3 _scale{1,1,1};

		bool GetActive() { return _isActive; }
		void SetActive(bool val) { _isActive = val; }

	private:
		bool _isActive{ true };
	};
}
