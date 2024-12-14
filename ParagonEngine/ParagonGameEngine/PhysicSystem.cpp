#include "PhysicSystem.h"

#include "SceneSystem.h"
#include "DebugSystem.h"
#include "PgLayer.h"
#include "DetectTriggers_PxQFC.h"

#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/StaticCapsuleCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/RayCast.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/CustomAssert.h"

//CPU Dispatcher : Core Count �˱� ����.
#include <thread>
#include <algorithm>

namespace Pg::Engine::Physic
{
	physx::PxFilterFlags ContactReportFilterShader(physx::PxFilterObjectAttributes
		attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32
		constantBlockSize)
	{
		using namespace physx;

		//Layer���� �浹 ����
		const bool maskTest = Pg::Engine::PgLayer::CanCollide(filterData0.word0, filterData1.word0);

		// Let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			if (maskTest)
			{
				// Notify trigger if masks specify it
				pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
				pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			}
			pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
			return PxFilterFlag::eDEFAULT;
		}


		// Send events for the kinematic actors but don't solve the contact
		if (PxFilterObjectIsKinematic(attributes0) && PxFilterObjectIsKinematic(attributes1))
		{
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
			return PxFilterFlag::eSUPPRESS;
		}

		// Trigger the contact callback for pairs (A,B) where the filtermask of A contains the ID of B and vice versa
		if (maskTest)
		{
			pairFlags |= PxPairFlag::eSOLVE_CONTACT;
			pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= PxPairFlag::ePOST_SOLVER_VELOCITY;
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;

			//LayerMask�� Ȱ��ȭ�Ǹ�, �� ���� Ȱ��� ��.
			// Ignore pair (no collisions nor events)
		}
		else
		{
			return PxFilterFlag::eKILL;
		}
	}

	void PhysicSystem::Initialize(Pg::Engine::DebugSystem* debugSystem)
	{
		//DebugSystem ������� ����.
		_debugSystem = debugSystem;

		//Custom Allocator �ݿ� w/ Custom Error Callback.
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);
		if (!_foundation) throw("PxCreateFoundation Failed!");


		// visual debugger ����, ���ÿ� ����
		// ip�ּ� �����ϴ°� ��û �Ž����µ�...�̰� ���?
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// ����, ����, ���� ���� ������ ���� ����
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		//Physics Callback ��ü ����.
		_physicsCallback = std::make_unique<PhysicsCallback>();

		//�浹 ���̾� ����.
		Pg::Engine::PgLayer::Clear();
		///���� : �ڽ� ���̾� // ������ : ���� ��ü�� �浹�� �� �ִ� ���̾�.
		///���� ��� Enum���� ������ ����. enum (int)

		//LayerMask (Pg::Data) ���� Ȱ���ؼ� CollisionLayer�� �����ϴ� �κ�.
		{
			//Unity�� LayerCollisionMatrix�� �Ųٷ� ���ٰ� �����ϸ� �ȴ�.
			//��ġ�� ��Ҵ� ���ְ�.
			using namespace Pg::Data::Enums;

			//CollisionMatrix�� �ݸ� ��� ����.
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_DEFAULT, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_PLAYER, { LAYER_PLAYER, LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_MONSTER, { LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_PROJECTILES, { });

			//��ü �׽���, ���α׷��Ӱ� 2���� �迭 ��ü ���� �°� Ŀ���س���. (-> ex. (0,1) �� 0bit�̶��, (1,0)�� �Ȱ��� 0bit���� �س��� ��!
			///Map�� ���̾ LAYER_DEFAULT�� ������ ����� ���� �߰��� �� �ʿ��� ����.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_DEFAULT, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER, LAYER_MAP, LAYER_BACKGROUND,LAYER_MOVABLE_OBJECTS, LAYER_BOSS });
			Pg::Engine::PgLayer::SetCollisionData(LAYER_PLAYER, { LAYER_DEFAULT,LAYER_MAP, LAYER_BACKGROUND,LAYER_MOVABLE_OBJECTS });
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MONSTER, { LAYER_DEFAULT, LAYER_MONSTER, LAYER_PROJECTILES, LAYER_MAP, LAYER_BACKGROUND, LAYER_MOVABLE_OBJECTS});
			Pg::Engine::PgLayer::SetCollisionData(LAYER_PROJECTILES, { LAYER_MONSTER, LAYER_BOSS }); //Projectile �������� �ڱ� �ڽŰ� �÷��̾�, �̷��� �浹 ���ϰ�. 
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MAP, { LAYER_DEFAULT, LAYER_PLAYER,   LAYER_MONSTER, LAYER_BOSS});
			Pg::Engine::PgLayer::SetCollisionData(LAYER_BACKGROUND, { LAYER_DEFAULT, LAYER_PLAYER,   LAYER_MONSTER, LAYER_BOSS}); //������ Layer 5���� �Ѵ�. �ٸ� ����� ���� X.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MOVABLE_OBJECTS, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER }); // ��� ������ �� �ִ� Movable Objects. Box / Door �� ����� ���̴�.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_BOSS, { LAYER_DEFAULT, LAYER_PROJECTILES, LAYER_BACKGROUND });
		}

		// ��Ƽ���� ����(����)
		//_material = _physics->createMaterial(0.5f, 0.5f, 0.5f);
		_material = _physics->createMaterial(0.5f, 0.5f, 0.0f); // Restitution : Bounciness�� ���� ��.

		CreatePxScene();

		// Pvd�� ���� ������
#ifdef _DEBUG
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif // DEBUG

		//Collider ����!
		InitMakeColliders();
	}

	void PhysicSystem::UpdatePhysics(float dTime)
	{
		//���� Scene�� 3D ��尡 �ƴϸ� �������� �ʴ´�.
		if (!_isScene3D)
		{
			return;
		}

		//�̸� �׿��� EventCallback Clear.
		_physicsCallback->Clear();

		accumulator += dTime;

		while (accumulator >= fixedDeltaTime)
		{
			_pxScene->simulate(fixedDeltaTime);

			_pxScene->fetchResults(true);

			accumulator -= fixedDeltaTime;
		}
		
		//Event �¾�.
		//Update�� �� ������ ��, Callback �Լ��� ������ �Լ��� ȣ��.
		_physicsCallback->CollectResults();
		_physicsCallback->SendTriggerEvents();
		_physicsCallback->SendCollisionEvents();

		//DynamicCollider ������Ʈ�� ���� ������Ʈ���� ���� ������Ʈ�� ����.
		//EventCallBack ������. ������ �״�� ����
		for (auto& rigid : _rigidDynamicVec)
		{
			Pg::Data::DynamicCollider* dynamicCol = static_cast<Pg::Data::DynamicCollider*>(rigid->userData);
			Pg::Data::GameObject* gameObj = dynamicCol->_object;

			//��Ÿ�ӿ� Collider �����״� ����.
			rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !dynamicCol->GetActive());


			if (!dynamicCol->GetTrigger())
			{
				if (!dynamicCol->GetWasCollided() && dynamicCol->GetIsCollide())
				{
					gameObj->OnCollisionEnter(dynamicCol->_collisionStorage.data(), dynamicCol->_collisionStorage.size());
					//
					// ("CollisionEnter!");
				}
				//Stay�� ��� �����ص״�. PhysX ���ο��� ���������� ����.
				else if (dynamicCol->GetWasCollided() && dynamicCol->GetIsCollide())
				{
					gameObj->OnCollisionStay();
					//PG_TRACE("CollisionStay!");
				}
				else if (dynamicCol->GetWasCollided() && !dynamicCol->GetIsCollide())
				{
					gameObj->OnCollisionExit(dynamicCol->_collisionStorage.data(), dynamicCol->_collisionStorage.size());
					//PG_TRACE("CollisionExit!");
				}
			}
			else
			{
				//Ʈ���� ����
				if (!dynamicCol->GetWasTrigger() && dynamicCol->GetIsTrigger())
				{
					gameObj->OnTriggerEnter(dynamicCol->_triggerStorage.data() , dynamicCol->_triggerStorage.size());
					//PG_TRACE("D-TriggerEnter!");
				}
				else if (dynamicCol->GetWasTrigger() && !dynamicCol->GetIsTrigger())
				{
					gameObj->OnTriggerExit(dynamicCol->_triggerStorage.data(), dynamicCol->_triggerStorage.size());
					//PG_TRACE("D-TriggerExit!");
				}
			}




		}

		//Static�� ���ؼ��� ���� ������Ʈ ����.
		for (auto& rigid : _rigidStaticVec)
		{
			Pg::Data::StaticCollider* staticCol = static_cast<Pg::Data::StaticCollider*>(rigid->userData);
			Pg::Data::GameObject* gameObj = staticCol->_object;

			rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !staticCol->GetActive());

			if (!staticCol->GetTrigger())
			{
				if (!staticCol->GetWasCollided() && staticCol->GetIsCollide())
				{
					assert(staticCol->_collisionStorage.size() >= 1);
					gameObj->OnCollisionEnter(staticCol->_collisionStorage.data(), staticCol->_collisionStorage.size());
					//PG_TRACE("CollisionEnter!");
				}
				//Stay�� ��� �����ص״�. PhysX ���ο��� ���������� ����.
				else if (staticCol->GetWasCollided() && staticCol->GetIsCollide())
				{
					gameObj->OnCollisionStay();
					//PG_TRACE("CollisionStay!");
				}
				else if (staticCol->GetWasCollided() && !staticCol->GetIsCollide())
				{
					gameObj->OnCollisionExit(staticCol->_collisionStorage.data(), staticCol->_collisionStorage.size());
					//PG_TRACE("CollisionExit!");
				}
			}
			else
			{
				//Ʈ���� ����
				if (!staticCol->GetWasTrigger() && staticCol->GetIsTrigger())
				{
					gameObj->OnTriggerEnter(staticCol->_triggerStorage.data(), staticCol->_triggerStorage.size());
					//PG_TRACE("S-TriggerEnter!");
				}
				else if (staticCol->GetWasTrigger() && !staticCol->GetIsTrigger())
				{
					gameObj->OnTriggerExit(staticCol->_triggerStorage.data(), staticCol->_triggerStorage.size());
					//PG_TRACE("S-TriggerExit!");
				}
			}
		}

		//PxTransform ������ ��ü ���� ������ Transform�� ����.
		Pg::Math::PGFLOAT3 position;
		Pg::Math::PGQuaternion quat;
		physx::PxTransform transform;

		for (auto& rigid : _rigidDynamicVec)
		{
			transform = rigid->getGlobalPose();

			position.x = transform.p.x;
			position.y = transform.p.y;
			position.z = transform.p.z;

			quat.x = transform.q.x;
			quat.y = transform.q.y;
			quat.z = transform.q.z;
			quat.w = transform.q.w;

			static_cast<Pg::Data::DynamicCollider*>(rigid->userData)->UpdatePhysics(position, quat);
		}
	}

	void PhysicSystem::UpdateTransform()
	{
		//���� Scene�� 3D ��尡 �ƴϸ� �������� �ʴ´�.
		if (!_isScene3D)
		{
			return;
		}

		for (auto& rigid : _rigidDynamicVec)
		{
			Pg::Data::DynamicCollider* dynamicCol = static_cast<Pg::Data::DynamicCollider*>(rigid->userData);
			dynamicCol->UpdateTransform();
		}

		for (auto& rigid : _rigidStaticVec)
		{
			Pg::Data::StaticCollider* staticCol = static_cast<Pg::Data::StaticCollider*>(rigid->userData);
			staticCol->UpdateTransform();
		}
	}


	void PhysicSystem::Finalize()
	{
		//Physx�� ���� ��� ���� �ʱ�ȭ.
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

		//���̾� ����.
		Pg::Engine::PgLayer::Clear();
	}

	void PhysicSystem::CreatePxScene()
	{
		if (_pxScene != nullptr)
		{
			PX_RELEASE(_pxScene);
		}

		// ���� ���� ����
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());

		//���� ���� ����
		physx::PxSweepHit sweepHit;

		//�߷� ����.
		//sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f); //x3


		//_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(std::clamp<unsigned int>(std::thread::hardware_concurrency() - 1, 1, 4)); //4������ ���. Like Flax.
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = ContactReportFilterShader;

		sceneDesc.simulationEventCallback = _physicsCallback.get();

		// Sub-Stepping ����
		//_pxScene->setSubStepCount(5, 1.0f / 60.0f);
		
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd�� ���� ������
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	///Pvd�� Stack�� ���� ����.
	void PhysicSystem::CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
	{
		//PxShape ����
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

	void PhysicSystem::InitMakeColliders()
	{
		PG_TRACE("Started Refreshing Colliders...");

		//�� ��ȯ �� �ݶ��̴� ���� ���� �� ��ε�.
		if (!_rigidDynamicVec.empty() || !_rigidStaticVec.empty())
		{
			_rigidDynamicVec.clear();
			_rigidStaticVec.clear();
		}

		//[TW] ������ Scene�� Physics Actor���� ���� ������ -> �� ����������.
		{
			using namespace physx;

			PxU32 nbActors = _pxScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
			PxActor** actors = new PxActor * [nbActors];

			UINT tActorBufferCount = _pxScene->getActors(
				physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, actors, nbActors);

			for (int i = 0; i < nbActors; i++)
			{
				PxActor* actor = actors[i];
				_pxScene->removeActor(*actor);
				PX_RELEASE(actor);
			}

			//���� �� ������ Ŭ����.
			delete[] actors;
		}


		//�̱���
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//���� ���� �����ϴ� ������Ʈ ����Ʈ�� �޾ƿ� 
		//Collider ������ ��� Collider�� ���� �����Ѵ�.
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			std::vector<Pg::Data::BoxCollider*> tBoxCol = obj->GetComponents<Pg::Data::BoxCollider>();
			std::vector<Pg::Data::SphereCollider*> tSphCol = obj->GetComponents<Pg::Data::SphereCollider>();
			std::vector<Pg::Data::CapsuleCollider*> tCapCol = obj->GetComponents<Pg::Data::CapsuleCollider>();
			std::vector<Pg::Data::PlaneCollider*> tPlaneCol = obj->GetComponents<Pg::Data::PlaneCollider>();
			std::vector<Pg::Data::StaticBoxCollider*> tStaticBoxCol = obj->GetComponents<Pg::Data::StaticBoxCollider>();
			std::vector<Pg::Data::StaticCapsuleCollider*> tStaticCapCol = obj->GetComponents<Pg::Data::StaticCapsuleCollider>();
			std::vector<Pg::Data::StaticSphereCollider*> tStaticSphCol = obj->GetComponents<Pg::Data::StaticSphereCollider>();

			//� Collider������ ���� ����� �����Ѵ�.
			if (!tBoxCol.empty())
			{
				MakeDynamicBoxCollider(obj);
			}
			else if (!tSphCol.empty())
			{
				MakeDynamicSphereCollider(obj);
			}
			else if (!tCapCol.empty())
			{
				MakeDynamicCapsuleCollider(obj);

			}
			else if (!tPlaneCol.empty())
			{
				MakePlaneCollider(obj);

			}
			else if (!tStaticBoxCol.empty())
			{
				MakeStaticBoxCollider(obj);
			}
			else if (!tStaticCapCol.empty())
			{
				MakeStaticCapsuleCollider(obj);
			}
			else if (!tStaticSphCol.empty())
			{
				MakeStaticShpereCollider(obj);
			}
		}

		//������� ��� Actor��, �Ѳ����� �߰��Ѵ�.
		AddAllObjectsToScene();

		PG_TRACE("...Ended Refreshing Colliders");

		//Scene ���� ���.
		
		this->_isScene3D = _sceneSystem->GetCurrentScene()->GetIs3D();
	}


	void PhysicSystem::MakeStaticBoxCollider(Pg::Data::GameObject* obj)
	{
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::StaticBoxCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::StaticBoxCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::StaticBoxCollider* staticBoxcol = dynamic_cast<Pg::Data::StaticBoxCollider*>(collider);

				physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(staticBoxcol->GetWidth() / 2.0f,
					staticBoxcol->GetHeight() / 2.0f, staticBoxcol->GetDepth() / 2.0f), *_material);

				// �浹 ������ ����
				boxShape->setContactOffset(0.02f); // ������ ������ ����
				boxShape->setRestOffset(0.01f);   // ������ ������ ����

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform worldTm = {position.x, position.y, position.z};

				boxShape->setLocalPose(trans);

				// Trigger ���� �Ǵ�
				if (staticBoxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				staticBoxcol->SetPxShape(boxShape);

				// Layer Mask ����
				boxShape->setSimulationFilterData({ staticBoxcol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*boxShape);

				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !staticBoxcol->GetActive());

				staticBoxcol->SetPxRigidStatic(rigid);
				rigid->userData = staticBoxcol;
				_rigidStaticVec.push_back(rigid);

				boxShape->release();
			}
		}
	}


	void PhysicSystem::MakeStaticCapsuleCollider(Pg::Data::GameObject* obj)
	{
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::StaticCapsuleCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::StaticCapsuleCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::StaticCapsuleCollider* staticCapCol = dynamic_cast<Pg::Data::StaticCapsuleCollider*>(collider);

				//physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(staticCapCol->GetRadius(), staticCapCol->GetHalfHeight()), *_material);
				//240609 : Scale�� Static�� ��� ��Ÿ�ӿ� ������ ������, Scale���̶� ���ϴ� ������� ����.
				//physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(staticCapCol->GetRadius() * staticCapCol->_object->_transform._scale.x, 
				//	staticCapCol->GetHalfHeight() * staticCapCol->_object->_transform._scale.y), *_material);
				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(staticCapCol->GetRadius() * staticCapCol->_object->_transform._scale.x,
					staticCapCol->GetHalfHeight()), *_material); // Half Height�� ���� Geometry������ ������ �Ǿ� ���� �ʴ�. Debugging Geometry�� �ϳ��� ������ ��� +a.

				// �浹 ������ ����
				shape->setContactOffset(2.f); // ������ ������ ����
				shape->setRestOffset(1.f);   // ������ ������ ����

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);

				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger ���� �Ǵ�
				if (staticCapCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}


				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				staticCapCol->SetPxShape(shape);

				// Layer Mask ����
				shape->setSimulationFilterData({ staticCapCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*shape);

				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !staticCapCol->GetActive());

				staticCapCol->SetPxRigidStatic(rigid);
				rigid->userData = staticCapCol;
				_rigidStaticVec.push_back(rigid);

				shape->release();
			}
		}
	}

	void PhysicSystem::MakeStaticShpereCollider(Pg::Data::GameObject* obj)
	{
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::StaticSphereCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::StaticSphereCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::StaticSphereCollider* staticSphCol = dynamic_cast<Pg::Data::StaticSphereCollider*>(collider);

				//physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(staticSphCol->GetRadius()), *_material);
				//240609 : Scale�� Static�� ��� ��Ÿ�ӿ� ������ ������, Scale���̶� ���ϴ� ������� ����.
				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(staticSphCol->GetRadius() * staticSphCol->_object->_transform._scale.x), *_material);

				// �浹 ������ ����
				shape->setContactOffset(2.f); // ������ ������ ����
				shape->setRestOffset(1.f);   // ������ ������ ����

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger ���� �Ǵ�
				if (staticSphCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				staticSphCol->SetPxShape(shape);

				// Layer Mask ����
				shape->setSimulationFilterData({ staticSphCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*shape);

				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !staticSphCol->GetActive());

				staticSphCol->SetPxRigidStatic(rigid);
				rigid->userData = staticSphCol;
				_rigidStaticVec.push_back(rigid);

				shape->release();

			}
		}
	}

	void PhysicSystem::MakeDynamicBoxCollider(Pg::Data::GameObject* obj)
	{
		//Collider�� Box�� ���� 
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::BoxCollider>();


		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::BoxCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::BoxCollider* boxcol = dynamic_cast<Pg::Data::BoxCollider*>(collider);

				//physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(boxcol->GetWidth() / 2.0f,
				//	boxcol->GetHeight() / 2.0f, boxcol->GetDepth() / 2.0f), *_material);
				//240609 : Scale�� Static�� ��� ��Ÿ�ӿ� ������ ������, Scale���̶� ���ϴ� ������� ����.
				//��Ÿ�ӿ� ������ �ʴ� ���� �������� ���. 
				physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry((boxcol->GetWidth() / 2.0f) * boxcol->_object->_transform._scale.x,
					(boxcol->GetHeight() / 2.0f) * boxcol->_object->_transform._scale.y, (boxcol->GetDepth() / 2.0f) * boxcol->_object->_transform._scale.z), *_material);

				// �浹 ������ ����
				boxShape->setContactOffset(0.02f); // ������ ������ ����
				boxShape->setRestOffset(0.01f);   // ������ ������ ����

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// ȸ�� �������� z������ 90�� ȸ����Ŵ
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				boxShape->setLocalPose(trans);
				//assert(trans.isValid());

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform worldTm(physx::PxVec3(position.x, position.y, position.z));

				//Ʈ���Ÿ� ���� PXShape ����.
				boxcol->SetPxShape(boxShape);

				//Trigger ���� �Ǵ�
				if (boxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}


				//�׽�Ʈ�� ���� �ӽ÷� Rigid �־.
				//�ӽ� �ƴ� �̷��� ���ļ� �� ����.
				//2023.12.11
				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(worldTm);

				//�浹 ��Ȯ�� ����� �ϱ����� �ڵ�
				rigid->setSolverIterationCounts(16, 4);

				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, collider->GetKinematic());
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !(collider->GetUseGravity()));

				// Layer Mask ����
				boxShape->setSimulationFilterData({ boxcol->GetLayer(), 0, 0, 0 });

				//Rigid�� �߷� ����
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(boxcol->GetLinearDamping());
				rigid->attachShape(*boxShape);

				//_pxScene->addActor(*rigid);
				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !boxcol->GetActive());

				boxcol->SetPxRigidDynamic(rigid);
				rigid->userData = boxcol;
				_rigidDynamicVec.push_back(rigid);

				boxShape->release();

			}

		}

	}

	void PhysicSystem::MakeDynamicSphereCollider(Pg::Data::GameObject* obj)
	{
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::SphereCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::SphereCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::SphereCollider* sphCol = dynamic_cast<Pg::Data::SphereCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(sphCol->GetRadius()), *_material);

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				
				// �浹 ������ ����
				//shape->setContactOffset(2.f); // ������ ������ ����
				//shape->setRestOffset(1.f);   // ������ ������ ����

				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// ȸ�� �������� z������ 90�� ȸ����Ŵ
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger ���� �Ǵ�
				if (sphCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				sphCol->SetPxShape(shape);

				// Layer Mask ����
				shape->setSimulationFilterData({ sphCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(worldTm);
				//rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
				rigid->setSolverIterationCounts(16, 4);

				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, collider->GetKinematic());
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !(collider->GetUseGravity()));

				//Rigid�� �߷� ����
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(sphCol->GetLinearDamping());
				rigid->attachShape(*shape);
				//_pxScene->addActor(*rigid);
				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !sphCol->GetActive());

				///collider�� �� ���� ��
				/*rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);*/

				sphCol->SetPxRigidDynamic(rigid);
				rigid->userData = sphCol;
				_rigidDynamicVec.push_back(rigid);
				shape->release();

			}
		}
	}

	void PhysicSystem::MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj)
	{
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::CapsuleCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::CapsuleCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::CapsuleCollider* capCol = dynamic_cast<Pg::Data::CapsuleCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(capCol->GetRadius(), capCol->GetHalfHeight()), *_material);

				//RotationOffset ����
				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				
				// �浹 ������ ����
				shape->setContactOffset(0.02f); // ������ ������ ����
				shape->setRestOffset(0.01f);   // ������ ������ ����

				physx::PxTransform trans(physx::PxIdentity);

				//quat = Pg::Math::PGConvertD3DQuatRotToPhysX(quat);

				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// ȸ�� �������� z������ 90�� ȸ����Ŵ
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset ����
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };
				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger ���� �Ǵ�
				if (capCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				capCol->SetPxShape(shape);

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				//auto pos = obj->_transform._position;
				//auto rot = obj->_transform._rotation;

				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));
				//physx::PxTransform worldTm;
				//worldTm.p = { pos.x , pos.y, pos.z };
				//worldTm.q = { rot.w, rot.x, rot.y, rot.z };

				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(worldTm);
				//rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
				rigid->setSolverIterationCounts(16, 4);

				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, collider->GetKinematic());
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !(collider->GetUseGravity()));

				// Layer Mask ����
				shape->setSimulationFilterData({ capCol->GetLayer(), 0, 0, 0 });

				//���ӵ� ����.
				physx::PxVec3 linearVelo;
				linearVelo.x = capCol->GetLinearVelocity().x;
				linearVelo.y = capCol->GetLinearVelocity().y;
				linearVelo.z = capCol->GetLinearVelocity().z;

				//Rigid�� �߷� ����
				rigid->setLinearVelocity(linearVelo);
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(capCol->GetLinearDamping());

				rigid->attachShape(*shape);

				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !capCol->GetActive());

				///collider�� �� ���� ��
				/*rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);*/

				//_pxScene->addActor(*rigid);

				//Rigid �����ϱ�
				capCol->SetPxRigidDynamic(rigid);
				rigid->userData = capCol;
				_rigidDynamicVec.push_back(rigid);
				shape->release();

			}
		}
	}

	void PhysicSystem::MakePlaneCollider(Pg::Data::GameObject* obj)
	{

		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::PlaneCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::PlaneCollider>();

			for (auto& collider : colliderVec)
			{
				///PxPlane�� �� �ȵǴ� �� ���� �ϴ� �ڽ�ó�� ����.
				///���Ŀ� PxPlane���� ����� ����.
				Pg::Data::PlaneCollider* planeCol = dynamic_cast<Pg::Data::PlaneCollider*>(collider);
				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(planeCol->GetWidth() / 2.0f, 0.1f, planeCol->GetDepth() / 2.0f), *_material);
				Pg::Math::PGFLOAT3 normal = planeCol->GetNormalVector();
				physx::PxTransform normalTm(physx::PxVec3(normal.x, normal.y, normal.z));
				//physx::PxPlane plane = { normal.x, normal.y, normal.z, planeCol->GetDistance() };

				// �浹 ������ ����
				shape->setContactOffset(2.f); // ������ ������ ����
				shape->setRestOffset(1.f);   // ������ ������ ����

				planeCol->SetPxShape(shape);

				// Layer Mask ����
				shape->setSimulationFilterData({ planeCol->GetLayer(), 0, 0, 0 });

				//Trigger ���� �Ǵ�
				if (planeCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				//physx::PxRigidStatic* rigid = PxCreatePlane(*_physics, plane, *_material);
				physx::PxRigidStatic* rigid = _physics->createRigidStatic(normalTm);
				rigid->attachShape(*shape);
				//Collider�� ���� ������ eDisableSimulation PhysX ���ο��� Ȱ��ȭ.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !planeCol->GetActive());

				planeCol->SetPxRigidStatic(rigid);
				rigid->userData = planeCol;

				_rigidStaticVec.push_back(rigid);
			}
		}
	}

	///Raycast �����ϱ�
	Pg::Data::Collider* PhysicSystem::MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, float tLength, Pg::Math::PGFLOAT3& outHitPoint, int* bType, bool detectTriggers)
	{
		physx::PxVec3 rayCastOrigin;
		rayCastOrigin.x = tOrigin.x;
		rayCastOrigin.y = tOrigin.y;
		rayCastOrigin.z = tOrigin.z;

		physx::PxVec3 rayCastDir;
		rayCastDir.x = tDir.x;
		rayCastDir.y = tDir.y;
		rayCastDir.z = tDir.z;

		Pg::Data::Collider* raycastCol = nullptr;

		//QueryFiltering (Trigger ���� ���� �˻�) - ���ǹ�.
		DetectTriggers_PxQFC tDetectTriggersCallback(detectTriggers);

		//RayCast ���� ����.
		physx::PxRaycastBuffer _hitBuffer;
		//Trigger �����ϴ��� �Էµ� ������ ���� ���͸�. 
		bool _isHit = _pxScene->raycast(rayCastOrigin, rayCastDir, tLength, _hitBuffer,
			physx::PxHitFlag::eDEFAULT, physx::PxQueryFilterData(physx::PxFilterData(), physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::ePREFILTER), &tDetectTriggersCallback);
		physx::PxVec3 tHitPoint = { 0.f,0.f,0.f };

		//���� RayCast�� �¾Ҵٸ�
		if (_isHit)
		{
			//PG_TRACE("RayCast Hit!");

			//�浹 ������Ʈ�� ������
			physx::PxRigidActor* actor = _hitBuffer.block.actor;

			//���� �� ���� �浹 ��ü ����
			if (actor->getType() == physx::PxActorType::eRIGID_STATIC)
			{
				actor = static_cast<physx::PxRigidStatic*>(actor);

				if (bType != nullptr)
				{
					*bType = 1;
				}
			}

			else if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				actor = static_cast<physx::PxRigidDynamic*>(actor);

				if (bType != nullptr)
				{
					*bType = 2;
				}
			}

			//�ǰ� ������ ����.
			raycastCol = static_cast<Pg::Engine::Collider*>(actor->userData);
			//PG_TRACE(raycastCol->_object->GetName());

			tHitPoint = _hitBuffer.block.position;

			//��������� HitPoint�� ��������.
			//�����ڷ� ������ ģ��.
			outHitPoint.x = tHitPoint.x;
			outHitPoint.y = tHitPoint.y;
			outHitPoint.z = tHitPoint.z;
		}

		//�ǰ� ��ü�� position �����͸� �̿��� �����.
		if (_debugSystem->GetDebugMode())
		{
			RayCastInfo tRayCastInfo;
			tRayCastInfo.isHit = _isHit;
			tRayCastInfo.hitPoint.x = tHitPoint.x;
			tRayCastInfo.hitPoint.y = tHitPoint.y;
			tRayCastInfo.hitPoint.z = tHitPoint.z;

			tRayCastInfo.dir = tDir;
			tRayCastInfo.origin = tOrigin;
			tRayCastInfo.length = tLength;
			_debugSystem->DrawRayCastDebug(tRayCastInfo);
		}

		return raycastCol;
	}


	void PhysicSystem::MakeSphereCast(const Pg::Math::PGFLOAT3& tOrigin, const Pg::Math::PGFLOAT3& tDir, float tRad, float max, unsigned int maxColCnt, Pg::Data::Collider** colDataPointer)
	{
		if (!_forSweepSphereInfo)
		{
			_forSweepSphereInfo = std::make_unique<Pg::Data::SphereInfo>();
		}

		//Sphere ��� Geometry
		physx::PxSphereGeometry sphereGeo(tRad);

		//Position ����
		physx::PxVec3 sphereCastOrigin(tOrigin.x, tOrigin.y, tOrigin.z);
		physx::PxVec3 sphereCastDir(tDir.x, tDir.y, tDir.z);
		physx::PxTransform shperePose(sphereCastOrigin);

		//�浹 ����
		physx::PxSweepBuffer sweepBuffer;

		bool isSweepHit = _pxScene->sweep(sphereGeo, shperePose, sphereCastDir, max, sweepBuffer);

		physx::PxVec3 tHitPoint;

		//���� RayCast�� �¾Ҵٸ�
		if (isSweepHit)
		{
			PG_TRACE("SphereCast Hit!");

			for (int i = 0; i < sweepBuffer.getNbTouches(); i++)
			{
				const physx::PxSweepHit& tTouch = sweepBuffer.getTouch(i);

				//�浹 ��ü�� ��ȿ�Ѱ�?
				if (tTouch.actor && tTouch.actor->userData)
				{
					//�ǰ� ������ ����.
					Pg::Data::Collider* sphereCastCol = static_cast<Pg::Engine::Collider*>(tTouch.actor->userData);

					if (i < maxColCnt)
					{
						colDataPointer[i] = sphereCastCol;
					}
					else
					{
						PG_TRACE("");
					}
				}
			}

			tHitPoint = sweepBuffer.block.position;
		}

		//�ǰ� ��ü�� position �����͸� �̿��� �����.
		if (_debugSystem->GetDebugMode())
		{
			_forSweepSphereInfo->scale.x = tRad * 2.0f;
			_forSweepSphereInfo->scale.y = tRad * 2.0f;
			_forSweepSphereInfo->scale.z = tRad * 2.0f;

			physx::PxMat44 startTM(shperePose);
			//startTM = startTM.getTranspose();
			memcpy(&_forSweepSphereInfo->worldTM, &startTM, sizeof(Pg::Math::PGFLOAT4X4));

			if (isSweepHit)
			{
				//Gold
				_forSweepSphereInfo->color = { 1.f, 0.843137324f, 0.f, 1.f };
			}
			else
			{
				//MediumVioletRed
				_forSweepSphereInfo->color = { 0.780392230f, 0.082352944f, 0.521568656f, 1.f };
			}
			_debugSystem->DrawSphereDebug(_forSweepSphereInfo.get());
		}
	}

	///������� Collider ��ü�� Scene���� �߰��ϴ� ����.
	void PhysicSystem::AddAllObjectsToScene()
	{
		//240617 �����丵, addActors�� ��ȯ.

		std::vector<physx::PxActor*> _actorCollection;

		for (auto& rigidDynamic : _rigidDynamicVec)
		{
			Pg::Data::DynamicCollider* dynamicCol = static_cast<Pg::Data::DynamicCollider*>(rigidDynamic->userData);
			dynamicCol->UpdateTransform();
			//_pxScene->addActor(*rigidDynamic);

			//���� ����� �� �� ���� ���Ž����ֱ� ����.
			//������ ����� Freeze �����ִ� ������ �Ѵ�.
			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, dynamicCol->GetAngularFreezeX());
			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, dynamicCol->GetAngularFreezeY());
			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, dynamicCol->GetAngularFreezeZ());

			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, dynamicCol->GetLinearFreezeX());
			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, dynamicCol->GetLinearFreezeY());
			rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, dynamicCol->GetLinearFreezeZ());
			
			_actorCollection.push_back(rigidDynamic);

		}

		for (auto& rigidStatic : _rigidStaticVec)
		{
			static_cast<Pg::Data::StaticCollider*>(rigidStatic->userData)->UpdateTransform();
			//_pxScene->addActor(*rigidStatic);
			_actorCollection.push_back(rigidStatic);
		}

		//�Ѳ����� �߰�.
		CustomAssert(_pxScene->addActors(_actorCollection.data(), _actorCollection.size()));
	}

	///�� �����Ӹ��� �浹 �̺�Ʈ ����
	void PhysicSystem::Flush()
	{
		//���� Scene�� 3D ��尡 �ƴϸ� �������� �ʴ´�.
		if (!_isScene3D)
		{
			return;
		}

		for (auto& rigid : _rigidDynamicVec)
		{
			static_cast<Pg::Data::DynamicCollider*>(rigid->userData)->Flush();
		}

		for (auto& rigid : _rigidStaticVec)
		{
			static_cast<Pg::Data::StaticCollider*>(rigid->userData)->Flush();
		}
	}

	void PhysicSystem::CheckAddRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec)
	{
		std::copy(vec->begin(), vec->end(), std::back_inserter(_tempAddedObjectsInPhysics));
	}

	void PhysicSystem::CheckDeleteRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec)
	{
		std::copy(vec->begin(), vec->end(), std::back_inserter(_tempDeletedObjectsInPhysics));
	}

	void PhysicSystem::ApplyRuntimeChangesCollider()
	{
		//���� Scene�� 3D ��尡 �ƴϸ� �������� �ʴ´�.
		if (!_isScene3D)
		{
			return;
		}

		for (auto& it : _tempAddedObjectsInPhysics)
		{
			ApplyAddSingleCollider(it);
		}

		for (auto& it : _tempDeletedObjectsInPhysics)
		{
			ApplyDeleteSingleCollider(it);
		}


		//Temporary List Clear.
		if (!_tempAddedObjectsInPhysics.empty())
		{
			_tempAddedObjectsInPhysics.clear();
		}

		if (!_tempDeletedObjectsInPhysics.empty())
		{
			_tempDeletedObjectsInPhysics.clear();
		}
	}

	void PhysicSystem::ApplyAddSingleCollider(Pg::Data::GameObject* obj)
	{
		///������ ���߿� Static Collider �߰��Ǹ� : �굵 ���ؾ�.
		//Vector�� MakeXXX �Լ� ���ο� Vector Add�� �־ �̰� �����丵 ��� ���� �߾�� �ؼ� ������.
		//���߿�, �ʿ��ϸ� �����ؾ�!


		//std::vector<Pg::Data::BoxCollider*> tBoxCol = obj->GetComponents<Pg::Data::BoxCollider>();
		//std::vector<Pg::Data::StaticBoxCollider*> tStaticBoxCol = obj->GetComponents<Pg::Data::StaticBoxCollider>();
		//std::vector<Pg::Data::SphereCollider*> tSphCol = obj->GetComponents<Pg::Data::SphereCollider>();
		//std::vector<Pg::Data::CapsuleCollider*> tCapCol = obj->GetComponents<Pg::Data::CapsuleCollider>();
		//std::vector<Pg::Data::PlaneCollider*> tPlaneCol = obj->GetComponents<Pg::Data::PlaneCollider>();
		//
		//if (!tBoxCol.empty())
		//{
		//	MakeDynamicBoxCollider(obj);
		//}
		//else if (!tStaticBoxCol.empty())
		//{
		//	MakeStaticBoxCollider(obj);
		//}
		//else if (!tSphCol.empty())
		//{
		//	MakeDynamicSphereCollider(obj);
		//}
		//else if (!tCapCol.empty())
		//{
		//	MakeDynamicCapsuleCollider(obj);
		//
		//}
		//else if (!tPlaneCol.empty())
		//{
		//	MakePlaneCollider(obj);
		//}
		//
		//
		////static_cast<Pg::Data::DynamicCollider*>(rigidDynamic->userData)->UpdateTransform();
		////_pxScene->addActor(*rigidDynamic);
		////
		////static_cast<Pg::Data::StaticCollider*>(rigidStatic->userData)->UpdateTransform();
		////_pxScene->addActor(*rigidStatic);

	}

	void PhysicSystem::ApplyDeleteSingleCollider(Pg::Data::GameObject* obj)
	{
		//�굵 �ٱ����� �ִ� ����Ʈ �������ָ� ���� (pxScene)
	}

}