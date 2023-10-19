#include "Collider.h"

namespace Pg::Data
{
	Collider::Collider(GameObject* owner) :
		Component(owner),
		_positionOffSet(0.0f, 0.0f, 0.0f)
	{

	}

	void Collider::SetPoisitonOffset(PGFLOAT3 position)
	{

	}

	void Collider::SetRotationOffset(PGQuaternion rotation)
	{

	}

	void Collider::SetScaleOffset(PGFLOAT3 scale)
	{

	}

	Pg::Math::PGFLOAT3 Collider::GetPositionOffset() const
	{
		return _positionOffSet;
	}

	Pg::Math::PGQuaternion Collider::GetRotationOffset() const
	{
		return _rotationOffset;
	}

	Pg::Math::PGFLOAT3 Collider::GetScaleOffset() const
	{
		return _scaleOffset;
	}
}