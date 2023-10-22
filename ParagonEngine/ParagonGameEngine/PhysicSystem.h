#pragma once

#include "Pxphysics.h"
#include "PxphysicsAPI.h"
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

namespace Pg::Engine::Physic
{
	class PhysicSystem
	{
	public:
		void Initialize();

		void UpdatePhysics();

		void Finalize();

	public:
		void CreatePxScene();

		void CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);

	public:


	private:
		
		//PhysX연동을 위한 변수들
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxTolerancesScale _toleranceScale;
		physx::PxFoundation* _foundation = nullptr;
		physx::PxPhysics* _physics = nullptr;
		physx::PxDefaultCpuDispatcher* _dispatcher = nullptr;
		physx::PxScene* _pxScene = nullptr;
		physx::PxMaterial* _material = nullptr;
		physx::PxPvd* _pvd = nullptr;
	};
}

