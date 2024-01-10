#include "StaticCollider.h"
#include "GameObject.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	StaticCollider::StaticCollider(GameObject* owner) :
		Collider(owner),
		_rigid(nullptr)
	{

	}

	void StaticCollider::Start()
	{

	}

	void StaticCollider::UpdateTransform()
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

	void StaticCollider::SetPxRigidStatic(physx::PxRigidStatic* rigid)
	{
		_rigid = rigid;
		//_pxScene = _rigid->getScene();
	}

	physx::PxRigidStatic* StaticCollider::GetPxRigidStatic()
	{
		return _rigid;
	}


}