#include "PhysicSystem.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Engine::Physic
{
	void PhysicSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);
		if (!_foundation) throw("PxCreateFoundation Failed!");

		// 버전, 세팅, 단위 등의 정보를 담은 물리
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		// visual debugger 세팅, 로컬에 연결
		// ip주소 설정하는거 엄청 거슬리는데...이걸 어떻게?
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("172.16.1.161", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

		CreatePxScene();

		// Pvd에 정보 보내기
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

	
		// 머티리얼 생성(임의)
		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);

		// ground 생성 후, 임의로 shape 붙여주기
		physx::PxRigidStatic* groundPlane = PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0), *_material);
		physx::PxShape* gpShape = _physics->createShape(physx::PxBoxGeometry(1.0f, 256.0f, 256.0f), *_material);
		groundPlane->attachShape(*gpShape);
		_pxScene->addActor(*groundPlane);

		/*for (physx::PxU32 i = 0; i < 5; i++)
		{
			CreateStack(physx::PxTransform(physx::PxVec3(0, 0,10.0f)), 10, 2.0f);
		}*/

		//예시로 도형 하나 만들기
		physx::PxRigidDynamic* exRigid = _physics->createRigidDynamic(physx::PxTransform(10.0f, 10.0f, 10.0f));
		physx::PxShape* exShape = _physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *_material);
		exRigid->attachShape(*exShape);
		_pxScene->addActor(*exRigid);
	}

	void PhysicSystem::UpdatePhysics()
	{
		_pxScene->simulate(1.0f / 60.0f);

		_pxScene->fetchResults(true);

		_pxScene->lockWrite();
		
		//PG_TRACE("PhysicSystem Updating...");
	}

	void PhysicSystem::Finalize()
	{
		//
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);

		if (_pvd)
		{
			physx::PxPvdTransport* transport = _pvd->getTransport();
			_pvd->release();
			PX_RELEASE(transport);
		}
		PX_RELEASE(_foundation);

		PG_TRACE("PhysicSystem released.");
	}

	void PhysicSystem::CreatePxScene()
	{
		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		// 임의로 중력을 2배로 했다.	3배로 늘림 ㅎㅎ
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd에 정보 보내기
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicSystem::CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
	{
		physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *_material);
		for (physx::PxU32 i = 0; i < size; i++)
		{
			for (physx::PxU32 j = 0; j < size - i; j++)
			{
				physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(t.transform(localTm));
				body->attachShape(*shape);
				physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
				_pxScene->addActor(*body);
			}
		}
		shape->release();
	}

	void PhysicSystem::MakeDynamicBoxCollider(Pg::Data::GameObject* obj)
	{
		//Collider를 Box로 설정 
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::BoxCollider>();

		auto colliderVec = obj->GetComponents<Pg::Data::BoxCollider>();

		for (auto& collider : colliderVec)
		{
			Pg::Data::BoxCollider* boxcol = dynamic_cast<Pg::Data::BoxCollider*>(collider);

			physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(boxcol->GetWidth() / 2 ,
				boxcol->GetHeight() / 2, boxcol->GetDepth() / 2), *_material);

			Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

			physx::PxTransform local(physx::PxVec3(position.x, position.y, position.z));

			boxShape->release();
		}

	}

	void PhysicSystem::MakeDynamicSphereCollider(Pg::Data::GameObject* obj)
	{

	}

	void PhysicSystem::MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj)
	{
		//Pg::Data::Collider* tmp = obj->GetComponent<Pg::Data::CapsuleCollider*>()
	}

	void PhysicSystem::CreateDynamicRigid(physx::PxShape* shape)
	{
		//RigidBody 생성(적용은 시키지 않음)
		//physx::PxRigidDynamic* rigid = _physics->createRigidDynamic();

	}

}