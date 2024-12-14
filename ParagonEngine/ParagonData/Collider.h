#pragma once
#include "Component.h"
#include "GameObject.h"
#include "../ParagonMath/PgMath.h"

#include <functional>
#include <PxPhysicsAPI.h>

namespace PG::Data
{
	class GameObject;
}

namespace physx
{
	class PxScene;
}

namespace Pg::Data
{
	class PhysicsCollision;
}

/// <summary>
/// �������� Collider ������Ʈ
/// 2D�� 3D�� �÷��� �����Ͽ� ������Ʈ�� �´� Collider ������ ����.
/// �ٵ� �ϴ� PhysX���� ����.....
/// 2023.10.17
/// </summary>
namespace Pg::Data
{
	using namespace Pg::Math;

	class Collider : public Component
	{
	public:
		Collider(GameObject* owner);

		virtual void BeforePhysicsAwake() final;

	public:
		PGFLOAT3 GetPositionOffset() const;
		PGQuaternion GetRotationOffset() const;
		PGFLOAT3 GetScaleOffset() const;

		PGFLOAT4X4 GetOffsetTranslateMatrix() const;
		PGFLOAT4X4 GetOffsetRotationMatrix() const;
		PGQuaternion GetOffsetRotationQuat() const;
		PGFLOAT4X4 GetOffsetScaleMatrix() const;

		void SetPositionOffset(PGFLOAT3 position);
		void SetRotationOffset(PGQuaternion rotation);
		void SetScaleOffset(PGFLOAT3 scale);

		PGFLOAT4X4 GetOffsetTM() const;

	public:
		void SetPxScene(physx::PxScene* scene );
		physx::PxScene* GetPxScene();

		virtual void UpdateTransform() abstract;

		void SetTrigger(bool isTrigger);
		bool GetTrigger();

		//�浹���� ���� üũ
		bool GetIsCollide();
		bool GetWasCollided();

		bool GetIsTrigger();
		bool GetWasTrigger();

		void SetLayer(uint32_t layer);
		uint32_t GetLayer();

		void Flush();

		void SetPxShape(physx::PxShape* shape);
	public:
		//API�� ���� �Լ�, GameObject�� ȣ��� ����.
		void Collider_OnCollisionEnter(PhysicsCollision& c);
		void Collider_OnCollisionExit(PhysicsCollision& c);
		void Collider_OnTriggerEnter(Collider* c);
		void Collider_OnTriggerExit(Collider* c);

		std::vector<PhysicsCollision*> _collisionStorage;
		std::vector<Collider*> _triggerStorage;

	protected:
		PGFLOAT3 _positionOffSet;
		PGQuaternion _rotationOffset;
		PGFLOAT3 _scaleOffset;

		physx::PxShape* _shape;
		physx::PxScene* _pxScene;

		bool _trigger;

		//std::vector�� ���� 1��n�� �浹�� ����ȭ�ϱ�.
		//void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count);
	protected:
		//�浹�� ���θ� �Ǵ��ϱ� ����.
		bool _isCollide;
		bool _wasCollided;

		//Ʈ������ ���θ� �Ǵ��ϱ� ����.
		bool _isTrigger;
		bool _wasTriggered;

		uint32_t _layer;
	};
}

