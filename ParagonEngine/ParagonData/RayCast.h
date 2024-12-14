#pragma once
#include "Collider.h"
#include "DebugData.h"

#include "../ParagonMath/PgMath.h"

/// <summary>
/// �������� RayCast ������Ʈ
/// ������ ��ũ���� �Ҷ� ��� �� �� �ִ� Ŭ������ ����� ������...
/// �ð� �������� ������Ʈ�� ����
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

		//RayCast�� ����
		PGFLOAT3 _origin;
		PGFLOAT3 _dir;
		float _length;
	};
}

