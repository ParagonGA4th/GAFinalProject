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


		///속도의 증감
		void SetVelocity(PGFLOAT3 velo);
		void AddVelocity(PGFLOAT3 velo);
		PGFLOAT3 GetVelocity() const;

		void SetAngularVelocity(PGFLOAT3 velo);
		PGFLOAT3 GetAngularVelocity() const;

		void SetLinearVelocity(PGFLOAT3 velo);
		PGFLOAT3 GetLinearVelocity() const;

	public:
		void AddForce(PGFLOAT3 dir, ForceMode mode);

		void SetLinearDamping(float val);
		float GetLinearDamping();
		
		//Move Rotation (리지드 바디가 해당 방향을 바라보게)
		//구조상 동작하지 않는 듯하다.. 
		void MoveRotation(PGQuaternion rot);

		///Collider의 축을 고정
		void FreezeAxisX(bool isActive);
		void FreezeAxisY(bool isActive);
		void FreezeAxisZ(bool isActive);

		void FreezeLinearX(bool isActive);
		void FreezeLinearY(bool isActive);
		void FreezeLinearZ(bool isActive);

		void SetMass(float value);
		void SetUseGravity(bool value);
		bool GetUseGravity();

		void SetKinematic(bool value);
		bool GetKinematic();

		bool GetAngularFreezeX() { return _isAngularFreezeX; }
		bool GetAngularFreezeY() { return _isAngularFreezeY; }
		bool GetAngularFreezeZ() { return _isAngularFreezeZ; }

		bool GetLinearFreezeX() { return _isLinearFreezeX; }
		bool GetLinearFreezeY() { return _isLinearFreezeY; }
		bool GetLinearFreezeZ() { return _isLinearFreezeZ; }

	public:

		//Collider 클래스에 적용되어있기에 가상함수로...
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	private:
		physx::PxRigidDynamic* _rigid{ nullptr };

	private:
		//플래그
		bool _isAngularFreezeX{false};
		bool _isAngularFreezeY{false};
		bool _isAngularFreezeZ{false};

		bool _isLinearFreezeX{false};
		bool _isLinearFreezeY{false};
		bool _isLinearFreezeZ{false};

		float _linearDamping;
		float _mass{ 1.0f };
		bool _useGravity{ true };
		bool _isKinematic{ false };

		//특정 방향으로의 중력값 조절.
		Pg::Math::PGFLOAT3 _linearVelocity;
	};
}

