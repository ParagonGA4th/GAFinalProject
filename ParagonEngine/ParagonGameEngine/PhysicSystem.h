#pragma once

//PhysX <-> CPP 교류를 위한 헤더.
#include "PhysicsCallback.h"
#include "PhysicsAllocator.h"
#include "PhysicsErrorCallback.h"

#include "Pxphysics.h"
#include "PxphysicsAPI.h"
#include <PxSimulationEventCallback.h>
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/LayerMask.h"
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

		void ApplyRuntimeChangesCollider();

		void UpdatePhysics(float dTime);
		void UpdateTransform();

		void Finalize();

		void AddAllObjectsToScene();

		void Flush();

	public:
		//PhysX 예제
		void CreatePxScene();

		void CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);	
	
	public:
		//Collider 전체 생성
		void InitMakeColliders();
		void CheckAddRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec);
		void CheckDeleteRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec);

		//StaticCollider 생성
		void MakeStaticBoxCollider(Pg::Data::GameObject* obj);
		void MakeStaticCapsuleCollider(Pg::Data::GameObject* obj);
		void MakeStaticShpereCollider(Pg::Data::GameObject* obj);
		void MakePlaneCollider(Pg::Data::GameObject* obj);
		
		//DynamicCollider 생성
		void MakeDynamicBoxCollider(Pg::Data::GameObject* obj);
		void MakeDynamicSphereCollider(Pg::Data::GameObject* obj);
		void MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj);
		
		//RayCast 생성
		//PARAGON_ENGINE_DLL Pg::Data::Collider* MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, 
		//	float tLength, int* bType);
		//히트한 포인트도 참조자로 리턴하게 설정, Collider가 Nullptr가 아닐 경우 참조자 리턴값은 내부에서 변경된 것이다.
		PARAGON_ENGINE_DLL Pg::Data::Collider* MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, float tLength, Pg::Math::PGFLOAT3& outHitPoint, int* bType, bool detectTriggers);
		PARAGON_ENGINE_DLL void MakeSphereCast(const Pg::Math::PGFLOAT3& tOrigin, const Pg::Math::PGFLOAT3& tDir, 
			float tRad, float max, unsigned int maxColCnt, Pg::Data::Collider** colDataPointer);

	private:
		void ApplyAddSingleCollider(Pg::Data::GameObject* obj);
		void ApplyDeleteSingleCollider(Pg::Data::GameObject* obj);

	private:
		//Rigid 정보를 담아놓는 벡터
		std::vector<physx::PxRigidDynamic*> _rigidDynamicVec;
		std::vector<physx::PxRigidStatic*> _rigidStaticVec;

	private:
		
		//PhysX연동을 위한 변수들
		//메모리 : 자체 양식으로 Allocate한다. From Flax.
		//physx::PxDefaultAllocator		_allocator;
		PhysicsAllocator				_allocator;

		//재정의, PG_WARN 형식으로 출력할 것.
		//physx::PxDefaultErrorCallback	_errorCallback;
		PhysicsErrorCallback			_errorCallback;
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

		std::unique_ptr<Pg::Data::SphereInfo> _forSweepSphereInfo{ nullptr };

		std::vector<Pg::Data::GameObject*> _tempAddedObjectsInPhysics;
		std::vector<Pg::Data::GameObject*> _tempDeletedObjectsInPhysics;

	private:
		const float fixedDeltaTime = 1.0f / 60.0f;
		float accumulator = 0.f;

	private:
		bool _isScene3D{ true };
	};
}

