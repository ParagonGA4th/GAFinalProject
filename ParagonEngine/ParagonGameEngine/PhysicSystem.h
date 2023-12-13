#pragma once

#include "Pxphysics.h"
#include "PxphysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonMath/PgMath.h"
#include <vector>

/// <summary>
/// ParagonEngine의 물리 시스템 클래스.
/// PhysX 연동하여 물리엔진을 연동할 것이다
/// 2023.10.06
/// </summary>
namespace Pg::Data
{
	class GameObject;
	class Transform;
}

namespace Pg::Engine
{
	class SceneSystem;
}

namespace Pg::Engine::Physic
{
	class PhysicSystem
	{
	public:
		void Initialize();

		void UpdatePhysics();

		void Finalize();

	public:
		//PhysX 예제
		void CreatePxScene();

		void CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);

	public:
		//Collider 전체 생성
		void MakeCollider();

		//StaticCollider 생성
		void MakeStaticBoxCollider(Pg::Data::GameObject* obj);
		void MakePlaneCollider(Pg::Data::GameObject* obj);
		
		//DynamicCollider 생성
		void MakeDynamicBoxCollider(Pg::Data::GameObject* obj);
		void MakeDynamicSphereCollider(Pg::Data::GameObject* obj);
		void MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj);

		//RigidBody 생성
		void CreateDynamicRigid(physx::PxShape* shape);

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

		Pg::Engine::SceneSystem*		_sceneSystem;
	};
}

