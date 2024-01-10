#pragma once
#include "Collider.h"
#include "ForceMode.h"
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
	class PxShape;
}

namespace Pg::Data
{
	using namespace Pg::Math;

	class DynamicCollider : public Collider
	{
	public:
		DynamicCollider(GameObject* owner);

	public:
		virtual void Start() override;

	public:
		void UpdatePhysics(PGFLOAT3 pos, PGQuaternion quat);
		virtual void UpdateTransform() override;

	public:
		void SetPxRigidDynamic(physx::PxRigidDynamic* rigid);
		physx::PxRigidDynamic* GetRigidBodyDynamic();

		void SetPxShape(physx::PxShape* shape);

		///속도의 증감
		void SetVelocity(PGFLOAT3 velo);
		void AddVelocity(PGFLOAT3 velo);
		PGFLOAT3 GetVelocity() const;
	public:
		void AddForce(PGFLOAT3 dir, ForceMode mode);
		
		///Collider의 축을 고정
		void FreezeAxisX(bool isActive);
		void FreezeAxisY(bool isActive);
		void FreezeAxisZ(bool isActive);
	public:

		//Collider 클래스에 적용되어있기에 가상함수로...
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	private:
		physx::PxRigidDynamic* _rigid;
		physx::PxShape* _shape;

	private:
		//플래그
		bool _isActiveX;
		bool _isActiveY;
		bool _isActiveZ;
	};
}

