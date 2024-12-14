#include "DynamicCollider.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	DynamicCollider::DynamicCollider(GameObject* owner) :
		Collider(owner),
		_isAngularFreezeX(false),
		_isAngularFreezeY(false),
		_isAngularFreezeZ(false),
		_linearDamping(0.f),
		_linearVelocity(0.f, 0.f, 0.f)
	{

	}

	void DynamicCollider::Start()
	{
		//각각의 축들을 Freeze 시켜주는 역할을 한다.
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _isAngularFreezeX);
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _isAngularFreezeY);
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _isAngularFreezeZ);
		//
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, _isLinearFreezeX);
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, _isLinearFreezeY);
		//_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, _isLinearFreezeZ);
	}

	void DynamicCollider::UpdatePhysics(PGFLOAT3 pos, PGQuaternion quat)
	{
		//GFLOAT4 localPos = PGFloat4MultiplyMatrix({ pos,1.0f }, GetOffsetTM().Inverse());
		PGFLOAT3 localPos = pos;
		PGQuaternion localQuat = quat;

		_object->_transform._position = { localPos.x, localPos.y, localPos.z };
		_object->_transform._rotation = localQuat;
	}

	void DynamicCollider::UpdateTransform()
	{
		//PxTransform을 자체 Transform과 연결시킨다.
		using namespace Pg::Math;

		//PGFLOAT4 position = PGFloat4MultiplyMatrix({ GetPositionOffset(), 1.f }, _object->_transform.GetWorldTM());
		PGFLOAT3 position = _object->_transform._position;

		PGQuaternion rotation = _object->_transform._rotation;
		//PGQuaternion rotation = Pg::Math::PGQuaternionMultiply(GetRotationOffset(), _object->_transform._rotation);


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

	void DynamicCollider::SetAngularVelocity(PGFLOAT3 velo)
	{
		_rigid->setAngularVelocity({ velo.x, velo.y, velo.z });
	}

	Pg::Math::PGFLOAT3 DynamicCollider::GetAngularVelocity() const
	{
		physx::PxVec3 vec = _rigid->getAngularVelocity();
		return { vec.x, vec.y, vec.z };
	}


	void DynamicCollider::AddVelocity(PGFLOAT3 velo)
	{
		physx::PxVec3 vec = { velo.x, velo.y , velo.z };
		vec += _rigid->getLinearVelocity();
		_rigid->setLinearVelocity(vec);
	}

	void DynamicCollider::FreezeAxisX(bool isActive)
	{
		_isAngularFreezeX = isActive;

		//바로 반영하게 고침, 240416.
		if (_rigid != nullptr)
		{
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _isAngularFreezeX);
		}
	}

	void DynamicCollider::FreezeAxisY(bool isActive)
	{
		_isAngularFreezeY = isActive;

		//바로 반영하게 고침, 240416.
		//TestScene 세팅을 고려. 
		if (_rigid != nullptr)
		{
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _isAngularFreezeY);
		}
	}

	void DynamicCollider::FreezeAxisZ(bool isActive)
	{
		_isAngularFreezeZ = isActive;

		if (_rigid != nullptr)
		{
			//바로 반영하게 고침, 240416.
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _isAngularFreezeZ);
		}
	}

	void DynamicCollider::SetLinearDamping(float val)
	{
		_linearDamping = val;

		if (_rigid != nullptr)
		{
			_rigid->setLinearDamping(val);
		}
	}

	float DynamicCollider::GetLinearDamping()
	{
		return _linearDamping;
	}

	//구조상 동작하지 않는 듯하다.. 
	void DynamicCollider::MoveRotation(PGQuaternion rot)
	{
		using namespace physx;
		PxTransform currentTransform = _rigid->getGlobalPose();

		// 로테이션 반영하는 새 PxTransform 만든다.
		PxQuat rotation = PxQuat(rot.x, rot.y, rot.z, rot.w); //우리가 원하는 Rotation.
		PxTransform newTransform = PxTransform(currentTransform.p, rotation);

		// 실제 Rigidbody에 세팅.
		_rigid->setGlobalPose(newTransform);
	}

	void DynamicCollider::SetMass(float value)
	{
		_mass = value;
		if (_rigid != nullptr)
		{
			_rigid->setMass(_mass);
		}
	}

	void DynamicCollider::SetUseGravity(bool value)
	{
		_useGravity = value;

		if (_rigid != nullptr)
		{
			_rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !value);
		}
	}

	void DynamicCollider::SetLinearVelocity(PGFLOAT3 velo)
	{
		_linearVelocity = velo;
	}

	Pg::Math::PGFLOAT3 DynamicCollider::GetLinearVelocity() const
	{
		return _linearVelocity;
	}

	void DynamicCollider::FreezeLinearX(bool isActive)
	{
		_isLinearFreezeX = isActive;

		//바로 반영하게 고침, 240416.
		if (_rigid != nullptr)
		{
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, _isLinearFreezeX);
		}
	}

	void DynamicCollider::FreezeLinearY(bool isActive)
	{
		_isLinearFreezeY = isActive;

		//바로 반영하게 고침, 240416.
		if (_rigid != nullptr)
		{
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, _isLinearFreezeY);
		}
	}

	void DynamicCollider::FreezeLinearZ(bool isActive)
	{
		_isLinearFreezeZ = isActive;

		//바로 반영하게 고침, 240416.
		if (_rigid != nullptr)
		{
			_rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, _isLinearFreezeZ);
		}
	}

	bool DynamicCollider::GetUseGravity()
	{
		return _useGravity;
	}

	void DynamicCollider::SetKinematic(bool value)
	{
		_isKinematic = value;
	}

	bool DynamicCollider::GetKinematic()
	{
		return _isKinematic;
	}
}