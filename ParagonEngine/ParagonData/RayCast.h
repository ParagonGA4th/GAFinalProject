#pragma once
#include "Collider.h"
#include "DebugData.h"

#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 RayCast 컴포넌트
/// 원래는 스크립팅 할때 사용 할 수 있는 클래스로 만들려 했으나...
/// 시간 부족으로 컴포넌트로 제작
/// 2024.01.17
/// </summary>

namespace physx
{
	class PxScene;
}

namespace Pg::Data
{
	using namespace Pg::Math; 
	
	class GameObject;

	class RayCast : public Collider
	{

	public:
		RayCast(GameObject* owner);

		virtual void Update() override;
		virtual void UpdateTransform() override;

		physx::PxScene* GetPxScene();
		void SetPxScene(physx::PxScene* scene);

		PGFLOAT3 GetOrigin();
		PGFLOAT3 GetDir();
		float GetLength();

		void SetOrigin(PGFLOAT3 origin);
		void SetDir(PGFLOAT3 dir);
		void SetLength(float length);

		
		//physx::PxRaycastBuffer
		
	public:
		RayCastInfo _rayCastInfo;

	private:

		physx::PxScene* _scene;

		//RayCast의 정보
		PGFLOAT3 _origin;
		PGFLOAT3 _dir;
		float _length;
	};
}

