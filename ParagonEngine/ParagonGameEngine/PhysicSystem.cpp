#include "PhysicSystem.h"
#include "../ParagonData/GameObject.h"


namespace Pg::Engine::Physic
{
	void PhysicSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// 버전, 세팅, 단위 등의 정보를 담은 물리
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		CreatePxScene();
	}

	void PhysicSystem::UpdatePhysics(float deltaTime)
	{

	}

	void PhysicSystem::Finalize()
	{

	}

	void PhysicSystem::CreatePxScene()
	{

	}

}