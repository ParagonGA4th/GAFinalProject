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

//CPU Dispatcher : Core Count 알기 위해.
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

		//Layer끼리 충돌 감지
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

			//LayerMask가 활성화되면, 이 역시 활용될 것.
			// Ignore pair (no collisions nor events)
		}
		else
		{
			return PxFilterFlag::eKILL;
		}
	}

	void PhysicSystem::Initialize(Pg::Engine::DebugSystem* debugSystem)
	{
		//DebugSystem 멤버변수 저장.
		_debugSystem = debugSystem;

		//Custom Allocator 반영 w/ Custom Error Callback.
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);
		if (!_foundation) throw("PxCreateFoundation Failed!");


		// visual debugger 세팅, 로컬에 연결
		// ip주소 설정하는거 엄청 거슬리는데...이걸 어떻게?
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// 버전, 세팅, 단위 등의 정보를 담은 물리
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		//_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		//Physics Callback 객체 생성.
		_physicsCallback = std::make_unique<PhysicsCallback>();

		//충돌 레이어 설정.
		Pg::Engine::PgLayer::Clear();
		///왼쪽 : 자신 레이어 // 오른쪽 : 왼쪽 객체와 충돌할 수 있는 레이어.
		///별도 헤더 Enum으로 구별할 예정. enum (int)

		//LayerMask (Pg::Data) 내부 활용해서 CollisionLayer를 설정하는 부분.
		{
			//Unity의 LayerCollisionMatrix를 거꾸로 본다고 생각하면 된다.
			//겹치는 요소는 없애고.
			using namespace Pg::Data::Enums;

			//CollisionMatrix의 반만 썼던 버전.
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_DEFAULT, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_PLAYER, { LAYER_PLAYER, LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_MONSTER, { LAYER_MONSTER, LAYER_PROJECTILES });
			//Pg::Engine::PgLayer::SetCollisionData(LAYER_PROJECTILES, { });

			//전체 테스팅, 프로그래머가 2차원 배열 전체 내용 맞게 커밋해놓자. (-> ex. (0,1) 이 0bit이라면, (1,0)도 똑같이 0bit으로 해놓는 것!
			///Map의 레이어가 LAYER_DEFAULT로 들어오는 관계로 수정 추가로 더 필요할 예정.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_DEFAULT, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER, LAYER_MAP, LAYER_BACKGROUND,LAYER_MOVABLE_OBJECTS, LAYER_BOSS });
			Pg::Engine::PgLayer::SetCollisionData(LAYER_PLAYER, { LAYER_DEFAULT,LAYER_MAP, LAYER_BACKGROUND,LAYER_MOVABLE_OBJECTS });
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MONSTER, { LAYER_DEFAULT, LAYER_MONSTER, LAYER_PROJECTILES, LAYER_MAP, LAYER_BACKGROUND, LAYER_MOVABLE_OBJECTS});
			Pg::Engine::PgLayer::SetCollisionData(LAYER_PROJECTILES, { LAYER_MONSTER, LAYER_BOSS }); //Projectile 기준으로 자기 자신과 플레이어, 이렇게 충돌 못하게. 
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MAP, { LAYER_DEFAULT, LAYER_PLAYER,   LAYER_MONSTER, LAYER_BOSS});
			Pg::Engine::PgLayer::SetCollisionData(LAYER_BACKGROUND, { LAYER_DEFAULT, LAYER_PLAYER,   LAYER_MONSTER, LAYER_BOSS}); //무조건 Layer 5여야 한다. 다른 대안은 존재 X.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_MOVABLE_OBJECTS, { LAYER_DEFAULT, LAYER_PLAYER, LAYER_MONSTER }); // 얘는 움직일 수 있는 Movable Objects. Box / Door 등 기믹일 것이다.
			Pg::Engine::PgLayer::SetCollisionData(LAYER_BOSS, { LAYER_DEFAULT, LAYER_PROJECTILES, LAYER_BACKGROUND });
		}

		// 머티리얼 생성(임의)
		//_material = _physics->createMaterial(0.5f, 0.5f, 0.5f);
		_material = _physics->createMaterial(0.5f, 0.5f, 0.0f); // Restitution : Bounciness를 줄인 것.

		CreatePxScene();

		// Pvd에 정보 보내기
#ifdef _DEBUG
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif // DEBUG

		//Collider 생성!
		InitMakeColliders();
	}

	void PhysicSystem::UpdatePhysics(float dTime)
	{
		//만약 Scene이 3D 모드가 아니면 실행하지 않는다.
		if (!_isScene3D)
		{
			return;
		}

		//미리 쌓였던 EventCallback Clear.
		_physicsCallback->Clear();

		accumulator += dTime;

		while (accumulator >= fixedDeltaTime)
		{
			_pxScene->simulate(fixedDeltaTime);

			_pxScene->fetchResults(true);

			accumulator -= fixedDeltaTime;
		}
		
		//Event 셋업.
		//Update가 다 끝났을 시, Callback 함수의 마무리 함수를 호출.
		_physicsCallback->CollectResults();
		_physicsCallback->SendTriggerEvents();
		_physicsCallback->SendCollisionEvents();

		//DynamicCollider 컴포넌트를 가진 오브젝트한테 물리 업데이트를 적용.
		//EventCallBack 적용함. 구조는 그대로 유지
		for (auto& rigid : _rigidDynamicVec)
		{
			Pg::Data::DynamicCollider* dynamicCol = static_cast<Pg::Data::DynamicCollider*>(rigid->userData);
			Pg::Data::GameObject* gameObj = dynamicCol->_object;

			//런타임에 Collider 껐다켰다 가능.
			rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !dynamicCol->GetActive());


			if (!dynamicCol->GetTrigger())
			{
				if (!dynamicCol->GetWasCollided() && dynamicCol->GetIsCollide())
				{
					gameObj->OnCollisionEnter(dynamicCol->_collisionStorage.data(), dynamicCol->_collisionStorage.size());
					//
					// ("CollisionEnter!");
				}
				//Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
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
				//트리거 감지
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

		//Static을 위해서도 물리 업데이트 적용.
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
				//Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
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
				//트리거 감지
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

		//PxTransform 정보를 자체 엔진 내부의 Transform과 연결.
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
		//만약 Scene이 3D 모드가 아니면 실행하지 않는다.
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
		//Physx에 관한 모든 변수 초기화.
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

		//레이어 해제.
		Pg::Engine::PgLayer::Clear();
	}

	void PhysicSystem::CreatePxScene()
	{
		if (_pxScene != nullptr)
		{
			PX_RELEASE(_pxScene);
		}

		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());

		//스윕 쿼리 설정
		physx::PxSweepHit sweepHit;

		//중력 설정.
		//sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f); //x3


		//_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(std::clamp<unsigned int>(std::thread::hardware_concurrency() - 1, 1, 4)); //4개까지 허용. Like Flax.
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = ContactReportFilterShader;

		sceneDesc.simulationEventCallback = _physicsCallback.get();

		// Sub-Stepping 설정
		//_pxScene->setSubStepCount(5, 1.0f / 60.0f);
		
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

	///Pvd에 Stack을 띄우는 예시.
	void PhysicSystem::CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
	{
		//PxShape 생성
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

		//씬 전환 시 콜라이더 전부 해제 후 재로드.
		if (!_rigidDynamicVec.empty() || !_rigidStaticVec.empty())
		{
			_rigidDynamicVec.clear();
			_rigidStaticVec.clear();
		}

		//[TW] 기존의 Scene에 Physics Actor들이 남아 있으면 -> 다 지워버린다.
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

			//버퍼 다 썼으면 클리어.
			delete[] actors;
		}


		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//현재 씬에 존재하는 오브젝트 리스트를 받아와 
		//Collider 존재할 경우 Collider를 전부 생성한다.
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			std::vector<Pg::Data::BoxCollider*> tBoxCol = obj->GetComponents<Pg::Data::BoxCollider>();
			std::vector<Pg::Data::SphereCollider*> tSphCol = obj->GetComponents<Pg::Data::SphereCollider>();
			std::vector<Pg::Data::CapsuleCollider*> tCapCol = obj->GetComponents<Pg::Data::CapsuleCollider>();
			std::vector<Pg::Data::PlaneCollider*> tPlaneCol = obj->GetComponents<Pg::Data::PlaneCollider>();
			std::vector<Pg::Data::StaticBoxCollider*> tStaticBoxCol = obj->GetComponents<Pg::Data::StaticBoxCollider>();
			std::vector<Pg::Data::StaticCapsuleCollider*> tStaticCapCol = obj->GetComponents<Pg::Data::StaticCapsuleCollider>();
			std::vector<Pg::Data::StaticSphereCollider*> tStaticSphCol = obj->GetComponents<Pg::Data::StaticSphereCollider>();

			//어떤 Collider인지에 따라 출력을 구분한다.
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

		//만들어진 모든 Actor들, 한꺼번에 추가한다.
		AddAllObjectsToScene();

		PG_TRACE("...Ended Refreshing Colliders");

		//Scene 상태 기록.
		
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

				// 충돌 오프셋 설정
				boxShape->setContactOffset(0.02f); // 적절한 값으로 설정
				boxShape->setRestOffset(0.01f);   // 적절한 값으로 설정

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform worldTm = {position.x, position.y, position.z};

				boxShape->setLocalPose(trans);

				// Trigger 여부 판단
				if (staticBoxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				staticBoxcol->SetPxShape(boxShape);

				// Layer Mask 설정
				boxShape->setSimulationFilterData({ staticBoxcol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*boxShape);

				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
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
				//240609 : Scale은 Static의 경우 런타임에 변하지 않으니, Scale값이랑 곱하는 방법으로 형성.
				//physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(staticCapCol->GetRadius() * staticCapCol->_object->_transform._scale.x, 
				//	staticCapCol->GetHalfHeight() * staticCapCol->_object->_transform._scale.y), *_material);
				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(staticCapCol->GetRadius() * staticCapCol->_object->_transform._scale.x,
					staticCapCol->GetHalfHeight()), *_material); // Half Height은 현재 Geometry적으로 연동이 되어 있지 않다. Debugging Geometry가 하나로 구성된 까닭 +a.

				// 충돌 오프셋 설정
				shape->setContactOffset(2.f); // 적절한 값으로 설정
				shape->setRestOffset(1.f);   // 적절한 값으로 설정

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);

				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger 여부 판단
				if (staticCapCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}


				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				staticCapCol->SetPxShape(shape);

				// Layer Mask 설정
				shape->setSimulationFilterData({ staticCapCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*shape);

				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
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
				//240609 : Scale은 Static의 경우 런타임에 변하지 않으니, Scale값이랑 곱하는 방법으로 형성.
				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(staticSphCol->GetRadius() * staticSphCol->_object->_transform._scale.x), *_material);

				// 충돌 오프셋 설정
				shape->setContactOffset(2.f); // 적절한 값으로 설정
				shape->setRestOffset(1.f);   // 적절한 값으로 설정

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger 여부 판단
				if (staticSphCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				staticSphCol->SetPxShape(shape);

				// Layer Mask 설정
				shape->setSimulationFilterData({ staticSphCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(worldTm);
				rigid->attachShape(*shape);

				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
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
		//Collider를 Box로 설정 
		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::BoxCollider>();


		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::BoxCollider>();

			for (auto& collider : colliderVec)
			{
				Pg::Data::BoxCollider* boxcol = dynamic_cast<Pg::Data::BoxCollider*>(collider);

				//physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(boxcol->GetWidth() / 2.0f,
				//	boxcol->GetHeight() / 2.0f, boxcol->GetDepth() / 2.0f), *_material);
				//240609 : Scale은 Static의 경우 런타임에 변하지 않으니, Scale값이랑 곱하는 방법으로 형성.
				//런타임에 변하지 않는 것을 기준으로 계산. 
				physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry((boxcol->GetWidth() / 2.0f) * boxcol->_object->_transform._scale.x,
					(boxcol->GetHeight() / 2.0f) * boxcol->_object->_transform._scale.y, (boxcol->GetDepth() / 2.0f) * boxcol->_object->_transform._scale.z), *_material);

				// 충돌 오프셋 설정
				boxShape->setContactOffset(0.02f); // 적절한 값으로 설정
				boxShape->setRestOffset(0.01f);   // 적절한 값으로 설정

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// 회전 오프셋을 z축으로 90도 회전시킴
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				boxShape->setLocalPose(trans);
				//assert(trans.isValid());

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform worldTm(physx::PxVec3(position.x, position.y, position.z));

				//트리거를 위해 PXShape 지정.
				boxcol->SetPxShape(boxShape);

				//Trigger 여부 판단
				if (boxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}


				//테스트를 위해 임시로 Rigid 넣어봄.
				//임시 아닌 이렇게 합쳐서 갈 예정.
				//2023.12.11
				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(worldTm);

				//충돌 정확도 계산을 하기위한 코드
				rigid->setSolverIterationCounts(16, 4);

				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, collider->GetKinematic());
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !(collider->GetUseGravity()));

				// Layer Mask 설정
				boxShape->setSimulationFilterData({ boxcol->GetLayer(), 0, 0, 0 });

				//Rigid의 중력 조정
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(boxcol->GetLinearDamping());
				rigid->attachShape(*boxShape);

				//_pxScene->addActor(*rigid);
				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
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
				
				// 충돌 오프셋 설정
				//shape->setContactOffset(2.f); // 적절한 값으로 설정
				//shape->setRestOffset(1.f);   // 적절한 값으로 설정

				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// 회전 오프셋을 z축으로 90도 회전시킴
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };

				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger 여부 판단
				if (sphCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform worldTm(physx::PxVec3(pos.x, pos.y, pos.z));

				sphCol->SetPxShape(shape);

				// Layer Mask 설정
				shape->setSimulationFilterData({ sphCol->GetLayer(), 0, 0, 0 });

				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(worldTm);
				//rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
				rigid->setSolverIterationCounts(16, 4);

				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, collider->GetKinematic());
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !(collider->GetUseGravity()));

				//Rigid의 중력 조정
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(sphCol->GetLinearDamping());
				rigid->attachShape(*shape);
				//_pxScene->addActor(*rigid);
				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !sphCol->GetActive());

				///collider의 축 고정 시
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

				//RotationOffset 설정
				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				
				// 충돌 오프셋 설정
				shape->setContactOffset(0.02f); // 적절한 값으로 설정
				shape->setRestOffset(0.01f);   // 적절한 값으로 설정

				physx::PxTransform trans(physx::PxIdentity);

				//quat = Pg::Math::PGConvertD3DQuatRotToPhysX(quat);

				trans.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

				// 회전 오프셋을 z축으로 90도 회전시킴
				physx::PxQuat rotation90(physx::PxPi / 2.0f, physx::PxVec3(0.0f, 0.0f, 1.0f));
				trans.q = trans.q * rotation90;

				//PositionOffset 설정
				auto offsetP = collider->GetPositionOffset();
				trans.p = { offsetP.x, offsetP.y , offsetP.z };
				shape->setLocalPose(trans);
				//assert(trans.isValid());

				//Trigger 여부 판단
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

				// Layer Mask 설정
				shape->setSimulationFilterData({ capCol->GetLayer(), 0, 0, 0 });

				//선속도 설정.
				physx::PxVec3 linearVelo;
				linearVelo.x = capCol->GetLinearVelocity().x;
				linearVelo.y = capCol->GetLinearVelocity().y;
				linearVelo.z = capCol->GetLinearVelocity().z;

				//Rigid의 중력 조정
				rigid->setLinearVelocity(linearVelo);
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(capCol->GetLinearDamping());

				rigid->attachShape(*shape);

				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !capCol->GetActive());

				///collider의 축 고정 시
				/*rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
				rigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);*/

				//_pxScene->addActor(*rigid);

				//Rigid 설정하기
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
				///PxPlane이 잘 안되는 것 같아 일단 박스처럼 생성.
				///추후에 PxPlane으로 출력할 예정.
				Pg::Data::PlaneCollider* planeCol = dynamic_cast<Pg::Data::PlaneCollider*>(collider);
				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(planeCol->GetWidth() / 2.0f, 0.1f, planeCol->GetDepth() / 2.0f), *_material);
				Pg::Math::PGFLOAT3 normal = planeCol->GetNormalVector();
				physx::PxTransform normalTm(physx::PxVec3(normal.x, normal.y, normal.z));
				//physx::PxPlane plane = { normal.x, normal.y, normal.z, planeCol->GetDistance() };

				// 충돌 오프셋 설정
				shape->setContactOffset(2.f); // 적절한 값으로 설정
				shape->setRestOffset(1.f);   // 적절한 값으로 설정

				planeCol->SetPxShape(shape);

				// Layer Mask 설정
				shape->setSimulationFilterData({ planeCol->GetLayer(), 0, 0, 0 });

				//Trigger 여부 판단
				if (planeCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				//physx::PxRigidStatic* rigid = PxCreatePlane(*_physics, plane, *_material);
				physx::PxRigidStatic* rigid = _physics->createRigidStatic(normalTm);
				rigid->attachShape(*shape);
				//Collider가 꺼져 있으면 eDisableSimulation PhysX 내부에서 활성화.
				rigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !planeCol->GetActive());

				planeCol->SetPxRigidStatic(rigid);
				rigid->userData = planeCol;

				_rigidStaticVec.push_back(rigid);
			}
		}
	}

	///Raycast 생성하기
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

		//QueryFiltering (Trigger 포함 여부 검사) - 조건문.
		DetectTriggers_PxQFC tDetectTriggersCallback(detectTriggers);

		//RayCast 버퍼 생성.
		physx::PxRaycastBuffer _hitBuffer;
		//Trigger 감지하는지 입력된 정보에 따라 필터링. 
		bool _isHit = _pxScene->raycast(rayCastOrigin, rayCastDir, tLength, _hitBuffer,
			physx::PxHitFlag::eDEFAULT, physx::PxQueryFilterData(physx::PxFilterData(), physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::ePREFILTER), &tDetectTriggersCallback);
		physx::PxVec3 tHitPoint = { 0.f,0.f,0.f };

		//만약 RayCast에 맞았다면
		if (_isHit)
		{
			//PG_TRACE("RayCast Hit!");

			//충돌 오브젝트의 포인터
			physx::PxRigidActor* actor = _hitBuffer.block.actor;

			//정적 및 동적 충돌 객체 구분
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

			//피격 데이터 전달.
			raycastCol = static_cast<Pg::Engine::Collider*>(actor->userData);
			//PG_TRACE(raycastCol->_object->GetName());

			tHitPoint = _hitBuffer.block.position;

			//명시적으로 HitPoint를 내보낸다.
			//참조자로 리턴할 친구.
			outHitPoint.x = tHitPoint.x;
			outHitPoint.y = tHitPoint.y;
			outHitPoint.z = tHitPoint.z;
		}

		//피격 객체의 position 데이터를 이용한 디버깅.
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

		//Sphere 모양 Geometry
		physx::PxSphereGeometry sphereGeo(tRad);

		//Position 설정
		physx::PxVec3 sphereCastOrigin(tOrigin.x, tOrigin.y, tOrigin.z);
		physx::PxVec3 sphereCastDir(tDir.x, tDir.y, tDir.z);
		physx::PxTransform shperePose(sphereCastOrigin);

		//충돌 버퍼
		physx::PxSweepBuffer sweepBuffer;

		bool isSweepHit = _pxScene->sweep(sphereGeo, shperePose, sphereCastDir, max, sweepBuffer);

		physx::PxVec3 tHitPoint;

		//만약 RayCast에 맞았다면
		if (isSweepHit)
		{
			PG_TRACE("SphereCast Hit!");

			for (int i = 0; i < sweepBuffer.getNbTouches(); i++)
			{
				const physx::PxSweepHit& tTouch = sweepBuffer.getTouch(i);

				//충돌 객체가 유효한가?
				if (tTouch.actor && tTouch.actor->userData)
				{
					//피격 데이터 전달.
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

		//피격 객체의 position 데이터를 이용한 디버깅.
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

	///만들어진 Collider 객체를 Scene으로 추가하는 역할.
	void PhysicSystem::AddAllObjectsToScene()
	{
		//240617 리팩토링, addActors로 전환.

		std::vector<physx::PxActor*> _actorCollection;

		for (auto& rigidDynamic : _rigidDynamicVec)
		{
			Pg::Data::DynamicCollider* dynamicCol = static_cast<Pg::Data::DynamicCollider*>(rigidDynamic->userData);
			dynamicCol->UpdateTransform();
			//_pxScene->addActor(*rigidDynamic);

			//씬이 재시작 될 때 마다 갱신시켜주기 위함.
			//각각의 축들을 Freeze 시켜주는 역할을 한다.
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

		//한꺼번에 추가.
		CustomAssert(_pxScene->addActors(_actorCollection.data(), _actorCollection.size()));
	}

	///매 프레임마다 충돌 이벤트 감지
	void PhysicSystem::Flush()
	{
		//만약 Scene이 3D 모드가 아니면 실행하지 않는다.
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
		//만약 Scene이 3D 모드가 아니면 실행하지 않는다.
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
		///별도로 나중에 Static Collider 추가되면 : 얘도 변해야.
		//Vector가 MakeXXX 함수 내부에 Vector Add가 있어서 이거 리팩토링 어느 정도 했어야 해서 빠꾸함.
		//나중에, 필요하면 연동해야!


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
		//얘도 바깥에서 있는 리스트 제거해주면 끝남 (pxScene)
	}

}