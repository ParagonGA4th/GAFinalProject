#include "DynamicCollider.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	DynamicCollider::DynamicCollider(GameObject* owner) :
		Collider(owner)
	{
		physx::PxTransform pxTransform;
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

	void DynamicCollider::SetPxRigidDynamic(void* rigid)
	{
		_rigid = rigid;
	}
}