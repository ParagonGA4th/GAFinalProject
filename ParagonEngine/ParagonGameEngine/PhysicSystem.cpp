//#include "PhysicSystem.h"
//#include "../ParagonData/GameObject.h"
//
//
//namespace Pg::Engine::Physic
//{
//	void PhysicSystem::Initialize()
//	{
//		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);
//
//		// 버전, 세팅, 단위 등의 정보를 담은 물리
//		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
//		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);
//
//		// visual debugger 세팅, 로컬에 연결
//		_pvd = PxCreatePvd(*_foundation);
//		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
//		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);
//
//		CreatePxScene();
//
//		// Pvd에 정보 보내기
//		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
//		if (pvdClient)
//		{
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//		}
//	
//		// 머티리얼 생성(임의)
//		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);
//	}
//
//	void PhysicSystem::UpdatePhysics(float deltaTime)
//	{
//
//	}
//
//	void PhysicSystem::Finalize()
//	{
//
//	}
//
//	void PhysicSystem::CreatePxScene()
//	{
//		// 씬에 대한 설정
//		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
//		// 임의로 중력을 2배로 했다.	3배로 늘림 ㅎㅎ
//		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f);
//		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
//		sceneDesc.cpuDispatcher = _dispatcher;
//		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
//		_pxScene = _physics->createScene(sceneDesc);
//
//		// Pvd에 정보 보내기
//		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
//		if (pvdClient)
//		{
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//		}
//	}
//
//}