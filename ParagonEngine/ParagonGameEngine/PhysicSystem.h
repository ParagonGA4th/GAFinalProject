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
namespace Pg::Engine::Physic
{
	class PhysicSystem
	{
	public:
		void Initialize();

		void UpdatePhysics(float deltaTime);

		void Finalize();

	public:
		void CreatePxScene();

	private:
		
		//PhysX연동을 위한 변수들
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;
	};
}

