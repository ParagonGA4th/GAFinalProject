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

		//PGFLOAT3 offsetP = GetPositionOffset();
		//PGFLOAT4 offsetPos4(offsetP.x, offsetP.y, offsetP.z, 1.0f);
		//PGFLOAT4 worldPos4 = offsetPos4 * _object->_transform.GetWorldTM();
		//PGFLOAT3 worldPos(worldPos4.x, worldPos4.y, worldPos4.z);

		//PGQuaternion rotationOffset = GetRotationOffset();
		//PGQuaternion worldRotation = _object->_transform._rotation;
		//PGQuaternion combinedRotation = PGQuaternionMultiply(worldRotation, rotationOffset);

		PGFLOAT3 position = _object->_transform._position;
		PGQuaternion rotation = _object->_transform._rotation;

		DirectX::XMMATRIX tWorld = PG2XM_MATRIX4X4(_object->_transform.GetWorldTM());
		DirectX::XMVECTOR tScale;
		DirectX::XMVECTOR tRot;
		DirectX::XMVECTOR tPos;
		DirectX::XMMatrixDecompose(&tScale, &tRot, &tPos, tWorld);
		position = XM2PG_FLOAT3_VECTOR(tPos);
		rotation = XM2PG_QUATERNION(tRot);

		physx::PxTransform transform;

		transform.p.x = position.x;
		transform.p.y = position.y;
		transform.p.z = position.z;

		transform.q.x = rotation.x;
		transform.q.y = rotation.y;
		transform.q.z = rotation.z;
		transform.q.w = rotation.w;

		//transform.q = physx::PxQuat(combinedRotation.x, combinedRotation.y, combinedRotation.z, combinedRotation.w);

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