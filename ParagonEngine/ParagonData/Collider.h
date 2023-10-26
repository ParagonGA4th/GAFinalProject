#pragma once
#include "Component.h"
#include "GameObject.h"
#include "../ParagonMath/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonMath.lib")
#endif //_DEBUG

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
		void SetPoisitonOffset(PGFLOAT3 position);
		void SetRotationOffset(PGQuaternion rotation);
		void SetScaleOffset(PGFLOAT3 scale);

		PGFLOAT3 GetPositionOffset() const;
		PGQuaternion GetRotationOffset() const;
		PGFLOAT3 GetScaleOffset() const;

		PGFLOAT4X4 GetOffsetTranslateMatrix() const;
		PGFLOAT4X4 GetOffsetRotationMatrix() const;
		PGFLOAT4X4 GetOffsetScaleMatrix() const;

	public:
		void SetPxScene(physx::PxScene* scene );
		physx::PxScene* GetPxScene();


	private:
		PGFLOAT3 _positionOffSet;
		PGQuaternion _rotationOffset;
		PGFLOAT3 _scaleOffset;

		physx::PxScene* _pxScene;
	};
}

