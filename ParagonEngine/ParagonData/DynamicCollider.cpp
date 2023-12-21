#include "DynamicCollider.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	DynamicCollider::DynamicCollider(GameObject* owner) :
		Collider(owner),
		_isCollide(false),
		_wasCollided(false),
		_isActiveX(false),
		_isActiveY(false),
		_isActiveZ(false)
	{
		
	}

	void DynamicCollider::Start()
	{
		_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _isActiveX);
		_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _isActiveY);
		_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _isActiveZ);
	}

	void DynamicCollider::UpdatePhysics(PGFLOAT3 pos, PGQuaternion quat)
	{



		PGFLOAT4 localPos = PGFloat4MultiplyMatrix({ pos, 1.0f }, GetOffsetTM().Inverse());
		PGQuaternion localQuat = PGQuaternionMultiply(quat, GetRotationOffset().Conjugate());

		_object->_transform.SetPosition(localPos.x, localPos.y, localPos.z);
		_object->_transform.SetRotation(localQuat);
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

		/*position.x = transform.p.x;
		position.y = transform.p.y;
		position.z = transform.p.z;

		rotation.x = transform.q.x;
		rotation.y = transform.q.y;
		rotation.z = transform.q.z;
		rotation.w = transform.q.w;*/

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
		direction.x = dir.x;
		direction.y = dir.y;
		direction.z = dir.z;

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

	void DynamicCollider::Flush()
	{
		_wasCollided = _isCollide;
		_isCollide = true;
	}

	void DynamicCollider::FreezeAxisX(bool isActive)
	{
		_isActiveX = isActive;
	}

	void DynamicCollider::FreezeAxisY(bool isActive)
	{
		_isActiveY = isActive;
	}

	void DynamicCollider::FreezeAxisZ(bool isActive)
	{
		_isActiveZ = isActive;
	}



}