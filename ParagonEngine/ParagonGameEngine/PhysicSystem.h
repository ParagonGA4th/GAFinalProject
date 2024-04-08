#pragma once

//PhysX <-> CPP 교류를 위한 헤더.
#include "PhysicsCallback.h"
#include "Pxphysics.h"
#include "PxphysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonMath/PgMath.h"
#include <vector>
#include <memory>

#include "EngineDLLExporter.h"

/// <summary>
/// ParagonEngine의 물리 시스템 클래스.
/// PhysX 연동하여 물리엔진을 만들 것이다
/// 2023.10.06
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class Collider;
}

namespace Pg::Engine
{
	class SceneSystem;
	class DebugSystem;
}

namespace Pg::Engine::Physic
{
	class PhysicSystem
	{
	public:
		void Initialize(Pg::Engine::DebugSystem* debugSystem);

		void UpdatePhysics(float dTime);
		void UpdateTransform();
		void UpdateRayCast();

		void Finalize();

		void AddObjectToScene();

		void Flush();

	public:
		//PhysX 예제
		void CreatePxScene();

		void CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);

	public:
		//Collider 전체 생성
		void InitMakeColliders();

		//StaticCollider 생성
		void MakeStaticBoxCollider(Pg::Data::GameObject* obj);
		void MakePlaneCollider(Pg::Data::GameObject* obj);
		
		//DynamicCollider 생성
		void MakeDynamicBoxCollider(Pg::Data::GameObject* obj);
		void MakeDynamicSphereCollider(Pg::Data::GameObject* obj);
		void MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj);
		
		//RayCast를 각각 다른 방식으로 구현해볼 예정.
		//void MakeRayCast(Pg::Data::GameObject* obj);

		//히트한 포인트도 참조자로 리턴하게 설정, Collider가 Nullptr가 아닐 경우 참조자 리턴값은 내부에서 변경된 것이다.
		PARAGON_ENGINE_DLL Pg::Data::Collider* MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, float tLength, Pg::Math::PGFLOAT3& outHitPoint, int* bType);
	private:
		//Rigid 정보를 담아놓는 벡터
		std::vector<physx::PxRigidDynamic*> _rigidDynamicVec;
		std::vector<physx::PxRigidStatic*> _rigidStaticVec;

	private:
		
		//PhysX연동을 위한 변수들
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxTolerancesScale		_toleranceScale;
		physx::PxPhysics*				_physics;
		physx::PxFoundation*			_foundation = nullptr;
		physx::PxDefaultCpuDispatcher*	_dispatcher = nullptr;
		physx::PxScene*					_pxScene = nullptr;
		physx::PxMaterial*				_material = nullptr;
		physx::PxPvd*					_pvd = nullptr;

		Pg::Engine::SceneSystem* _sceneSystem;
		Pg::Engine::DebugSystem* _debugSystem = nullptr;

		std::unique_ptr<PhysicsCallback> _physicsCallback;
	};
}

