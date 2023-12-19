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
		eFORCE,
		eIMPULSE,
		eACCELERATION,
		eVELOCITY_CHANGE
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
	using namespace Pg::Math;

	class DynamicCollider : public Collider
	{
	public:
		DynamicCollider(GameObject* owner);

	public:
		virtual void Start() abstract;

	public:
		void UpdateTransform();

	public:
		//충돌판정 여부 체크
		bool GetIsCollide();
		bool GetWasCollided();

		void SetPxRigidDynamic(physx::PxRigidDynamic* rigid);
		physx::PxRigidDynamic* GetRigidBodyDynamic();

		void SetVelocity(PGFLOAT3 velo);
		PGFLOAT3 GetVelocity() const;

	public:
		void AddForce(PGFLOAT3 dir, ForceMode mode);

	public:

		//Collider 클래스에 적용되어있기에 가상함수로...
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	private:
		physx::PxRigidDynamic* _rigid;

		//충돌의 여부를 판단하기 위해.
		bool _isCollide;
		bool _wasCollided;
	};
}

