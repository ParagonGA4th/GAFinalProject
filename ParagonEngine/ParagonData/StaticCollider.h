#pragma once
#include "Collider.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// Rigid가 적용되지 않는 Collider.
/// 아마 Box와 Plane정도만 구현하지 않을까....
/// 2023.12.11
/// </summary>

namespace physx
{
	class PxRigidStatic;
}

namespace Pg::Data
{
	class GameObject;

	class StaticCollider : public Collider
	{
	public:
		StaticCollider(GameObject* owner);

	public:
		virtual void Start() override;

	public:
		void UpdateTransform();

	public:
		void SetPxRigidStatic(physx::PxRigidStatic* rigid);
		physx::PxRigidStatic* GetPxRigidStatic();

	private:
		physx::PxRigidStatic* _rigid;
	};
}

