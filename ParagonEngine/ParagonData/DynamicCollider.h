#pragma once
#include "Collider.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 DynamicCollider.
/// 2023.10.23
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

namespace physx
{
	class PxRigidDynamic;
}

namespace Pg::Data
{
	class DynamicCollider : public Collider
	{
	public:
		DynamicCollider(GameObject* owner);

	public:
		//Collider 클래스에 적용되어있기에 가상함수로...
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	private:
		physx::PxRigidDynamic* _rigid;
	};
}

