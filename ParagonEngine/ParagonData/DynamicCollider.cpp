#include "DynamicCollider.h"

namespace Pg::Data
{
	DynamicCollider::DynamicCollider(GameObject* owner) :
		Collider(owner)
	{

	}

	bool DynamicCollider::SetIsCollided()
	{

		return true;
	}

	bool DynamicCollider::GetIsCollided()
	{
		return true;
	}

	void DynamicCollider::AddForce(PGFLOAT3 dir, ForceMode mode)
	{

	}
}