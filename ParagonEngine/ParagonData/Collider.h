#pragma once
#include "Component.h"
#include "GameObject.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 Collider 컴포넌트
/// 2D와 3D에 플래그 적용하여 오브젝트에 맞는 Collider 적용할 예정.
/// 근데 일단 PhysX부터 어찌.....
/// 2023.10.17
/// </summary>

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
	using namespace Pg::Math;

	class Collider : public Component
	{
	public:
		Collider(GameObject* owner);

	public:

		PGFLOAT3 GetPositionOffset() const;
		PGQuaternion GetRotationOffset() const;
		PGFLOAT3 GetScaleOffset() const;

		PGFLOAT4X4 GetOffsetTranslateMatrix() const;
		PGFLOAT4X4 GetOffsetRotationMatrix() const;
		PGFLOAT4X4 GetOffsetScaleMatrix() const;

		void SetPoisitonOffset(PGFLOAT3 position);
		void SetRotationOffset(PGQuaternion rotation);
		void SetScaleOffset(PGFLOAT3 scale);

		PGFLOAT4X4 GetOffsetTM() const;

	public:
		void SetPxScene(physx::PxScene* scene );
		physx::PxScene* GetPxScene();

		virtual void UpdateTransform() abstract;

		void SetTrigger(bool isTrigger);
		bool GetTrigger();

		//충돌판정 여부 체크
		void Collide();
		bool GetIsCollide();
		bool GetWasCollided();

	protected:
		PGFLOAT3 _positionOffSet;
		PGQuaternion _rotationOffset;
		PGFLOAT3 _scaleOffset;

		physx::PxScene* _pxScene;

		bool _isTrigger;

	protected:
		//충돌의 여부를 판단하기 위해.
		bool _isCollide;
		bool _wasCollided;
	};
}

