#pragma once
#include "Collider.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 DynamicCollider.
/// 2023.10.23
/// </summary>

namespace Pg::Data
{
	//힘을 가하는 방법
	enum class ForceMode : int
	{
		FORCE,
		IMPULSE,
		ACCELERATION
	};
}

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

		//충돌판정 여부 체크
		bool SetIsCollided();
		bool GetIsCollided();

		void SetPxRigidDynamic(void* rigid);

	public:
		void AddForce(PGFLOAT3 dir, ForceMode mode);

	public:

		//Collider 클래스에 적용되어있기에 가상함수로...
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	private:
		void* _rigid;

		bool _isCollided;
	};
}

