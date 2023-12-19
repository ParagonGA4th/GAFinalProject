#include "DynamicCollider.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	DynamicCollider::DynamicCollider(GameObject* owner) :
		Collider(owner),
		_isCollide(false),
		_wasCollided(false)
	{
		
	}


	void DynamicCollider::UpdateTransform()
	{
		using namespace Pg::Math;

		PGFLOAT4 position = PGFLOAT4(GetPositionOffset(), 1.0f) * _object->_transform.GetWorldTM();
		PGQuaternion rotation = PGQuaternionMultiply(GetRotationOffset(), _object->_transform.GetRotation());
		
		physx::PxTransform transform;

		transform.p.x = position.x;
		transform.p.y = position.y;
		transform.p.z = position.z;

		transform.q.x = rotation.x;
		transform.q.y = rotation.y;
		transform.q.z = rotation.z;
		transform.q.w = rotation.w;

		_rigid->setGlobalPose(transform);
	}

	bool DynamicCollider::GetIsCollide()
	{
		return _isCollide;
	}

	bool DynamicCollider::GetWasCollided()
	{
		return _wasCollided;
	}

	void DynamicCollider::AddForce(PGFLOAT3 dir, ForceMode mode)
	{
		physx::PxVec3 direction;
		dir.x = direction.x;
		dir.y = direction.y;
		dir.z = direction.z;

		//Force의 방향에 따라 구분
		switch (mode)
		{
			case ForceMode::eFORCE:
				_rigid->addForce(direction, physx::PxForceMode::eFORCE);
				break;
			case ForceMode::eIMPULSE:
				_rigid->addForce(direction, physx::PxForceMode::eIMPULSE);
				break;
			case ForceMode::eACCELERATION:
				_rigid->addForce(direction, physx::PxForceMode::eACCELERATION);
				break;
			case ForceMode::eVELOCITY_CHANGE:
				_rigid->addForce(direction, physx::PxForceMode::eVELOCITY_CHANGE);
				break;
		}
	}

	void DynamicCollider::SetPxRigidDynamic(physx::PxRigidDynamic* rigid)
	{
		_rigid = rigid;
	}

	physx::PxRigidDynamic* DynamicCollider::GetRigidBodyDynamic()
	{
		return _rigid;
	}

	void DynamicCollider::SetVelocity(PGFLOAT3 velo)
	{
		_rigid->setLinearVelocity({ velo.x, velo.y, velo.z });
	}

	Pg::Math::PGFLOAT3 DynamicCollider::GetVelocity() const
	{
		physx::PxVec3 vec = _rigid->getLinearVelocity();
		return { vec.x, vec.y, vec.z };
	}
}