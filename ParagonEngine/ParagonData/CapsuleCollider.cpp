#include "CapsuleCollider.h"

namespace Pg::Data
{
	CapsuleCollider::CapsuleCollider(GameObject* owner) :
		DynamicCollider(owner),
		_radius(1.0f),
		_halfHeight(1.0f)
	{

	}

	void CapsuleCollider::Update()
	{
		Pg::Math::PGFLOAT4X4 worldTM = GetOffsetTM() * _object->_transform.GetWorldTM();
	}

	float CapsuleCollider::GetWidth() const
	{
		return 0;
	}

	float CapsuleCollider::GetHeight() const
	{
		return 0;
	}

	float CapsuleCollider::GetDepth() const
	{
		return 0;
	}

	float CapsuleCollider::GetRadius() const
	{
		return _radius;
	}

	float CapsuleCollider::GetHalfHeight() const
	{
		return _halfHeight;
	}
}