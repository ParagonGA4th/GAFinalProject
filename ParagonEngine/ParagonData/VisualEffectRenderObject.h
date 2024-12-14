#pragma once
#include "../ParagonMath/PgMath.h"

//Effect Render Object : 쉽게 만들어지고 / 삭제될 수 있도록 만들어짐.

namespace Pg::Data
{
	struct VisualEffectRenderObject
	{
	public:
		//IsFaceCamera / Is3DSpace 값에 따라서 영향을 받을 것.
		//Ex. IsFaceCamera면 Rotation이 반영이 안됨.
		//Ex. Is3DSpace가 False면, Position은 X,Y만 쓰이고 .. etc.
		Pg::Math::PGFLOAT3 _position{0,0,0};
		Pg::Math::PGQuaternion _rotation{1,0,0,0};
		Pg::Math::PGFLOAT3 _scale{1,1,1};

		bool GetActive() { return _isActive; }
		void SetActive(bool val) { _isActive = val; }

	private:
		bool _isActive{ true };
	};
}
