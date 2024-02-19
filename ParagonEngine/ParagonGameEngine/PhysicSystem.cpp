#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"

#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/RayCast.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Engine::Physic
{

	physx::PxFilterFlags ContactReportFilterShader(physx::PxFilterObjectAttributes
		attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32
		constantBlockSize)
	{
		using namespace physx;
		
		//원래는 이 공간에 LayerMask가 있어야 한다.
		//const bool maskTest = (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1);

		// Let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			// 트리거 Notify (일단은 Masking 없이)
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
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
		pairFlags |= PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		pairFlags |= PxPairFlag::ePOST_SOLVER_VELOCITY;
		pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		return PxFilterFlag::eDEFAULT;

		//LayerMask가 활성화되면, 이 역시 활용될 것.
		// Ignore pair (no collisions nor events)
		//return PxFilterFlag::eKILL;
	}

	void PhysicSystem::Initialize(Pg::Engine::DebugSystem* debugSystem)
	{
		//DebugSystem 멤버변수 저장.
		_debugSystem = debugSystem;

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
		_material = _physics->createMaterial(0.1f, 0.1f, 0.5f);

		///RayCast의 예시
		//physx::PxVec3 origin = { 0.0f,0.0f,0.0f };		// [in] Ray origin
		//physx::PxVec3 unitDir = { 10.0f, 10.0f, 10.0f };	// [in] Normalized ray direction
		//physx::PxReal maxDistance = 100.0f;				// [in] Raycast max distance

		//const physx::PxU32 bufferSize = 256;			// [in] size of 'hitBuffer'
		//physx::PxRaycastHit hitBuffer[bufferSize];		// [out] User provided buffer for results
		//physx::PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here


		//const physx::PxRenderBuffer& rb = _pxScene->getRenderBuffer();

		//for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
		//{
		//	const physx::PxDebugLine& lineDebug = rb.getLines()[i];
		//}

		//bool hit = _pxScene->raycast(origin, unitDir, maxDistance, buf);

		//if (hit)
		//{
		//	PG_TRACE("Hit!!");
		//}

		//Collider 생성!
		InitMakeColliders();

		// ground 생성 후, 임의로 shape 붙여주기
		/*physx::PxRigidStatic* groundPlane = PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0), *_material);
		physx::PxShape* gpShape = _physics->createShape(physx::PxBoxGeometry(0.1f, 20.0f, 20.0f), *_material);
		groundPlane->attachShape(*gpShape);
		_pxScene->addActor(*groundPlane);*/

		/*for (physx::PxU32 i = 0; i < 5; i++)
		{
			CreateStack(physx::PxTransform(physx::PxVec3(0, 0,10.0f)), 10, 2.0f);
		}*/

		//예시로 도형 하나 만들기
		/*physx::PxRigidDynamic* exRigid = _physics->createRigidDynamic(physx::PxTransform(10.0f, 10.0f, 10.0f));
		physx::PxShape* exShape = _physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *_material);
		exRigid->attachShape(*exShape);
		_pxScene->addActor(*exRigid);*/
	}

	void PhysicSystem::UpdatePhysics(float dTime)
	{
		//미리 쌓였던 EventCallback Clear.
		_physicsCallback->Clear();

		_pxScene->simulate(dTime);

		_pxScene->fetchResults(true);

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

			if (!dynamicCol->GetWasCollided() && dynamicCol->GetIsCollide())
			{
				gameObj->OnCollisionEnter(dynamicCol->_collisionStorage.data(), dynamicCol->_collisionStorage.size());
				PG_TRACE("CollisionEnter!");
			}
			//Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
			else if (dynamicCol->GetWasCollided() && dynamicCol->GetIsCollide())
			{
				gameObj->OnCollisionStay();
				PG_TRACE("CollisionStay!");
			}
			else if (dynamicCol->GetWasCollided() && !dynamicCol->GetIsCollide())
			{
				gameObj->OnCollisionExit(dynamicCol->_collisionStorage.data(), dynamicCol->_collisionStorage.size());
				PG_TRACE("CollisionExit!");
			}

			//트리거 감지를 위해 잠시 해둠
			if (dynamicCol->GetTrigger() == true)
			{
				//gameObj->OnTriggerStay();
				//PG_TRACE("TriggerStay!");
			}
		}

		//Static을 위해서도 물리 업데이트 적용.
		for (auto& rigid : _rigidStaticVec)
		{
			Pg::Data::StaticCollider* staticCol = static_cast<Pg::Data::StaticCollider*>(rigid->userData);
			Pg::Data::GameObject* gameObj = staticCol->_object;

			if (!staticCol->GetWasCollided() && staticCol->GetIsCollide())
			{
				assert(staticCol->_collisionStorage.size() >= 1);
				gameObj->OnCollisionEnter(staticCol->_collisionStorage.data(), staticCol->_collisionStorage.size());
				PG_TRACE("CollisionEnter!");
			}
			//Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
			else if (staticCol->GetWasCollided() && staticCol->GetIsCollide())
			{
				gameObj->OnCollisionStay();
				PG_TRACE("CollisionStay!");
			}
			else if (staticCol->GetWasCollided() && !staticCol->GetIsCollide())
			{
				gameObj->OnCollisionExit(staticCol->_collisionStorage.data(), staticCol->_collisionStorage.size());
				PG_TRACE("CollisionExit!");
			}

			//트리거 감지를 위해 잠시 해둠
			if (staticCol->GetTrigger() == true)
			{
				//gameObj->OnTriggerStay();
				//PG_TRACE("TriggerStay!");
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


		//rayCast는 매 프레임마다 받아와야 하므로 여기다가 임시로 해본다.
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::RayCast* tRayCast = obj->GetComponent<Pg::Data::RayCast>();

			if (tRayCast != nullptr)
			{
				//MakeRayCast(obj);
			}
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
	}

	void PhysicSystem::CreatePxScene()
	{
		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());

		//중력 설정.
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = ContactReportFilterShader;
		//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		//sceneDesc.filterShader = FilterShader;
		sceneDesc.simulationEventCallback = _physicsCallback.get();

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
		//씬 전환 시 콜라이더 전부 해제 후 재로드.
		if (!_rigidDynamicVec.empty() || !_rigidStaticVec.empty())
		{
			_rigidDynamicVec.clear();
			_rigidStaticVec.clear();
		}

		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//현재 씬에 존재하는 오브젝트 리스트를 받아와 
		//Collider 존재할 경우 Collider를 전부 생성한다.
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			std::vector<Pg::Data::BoxCollider*> tBoxCol = obj->GetComponents<Pg::Data::BoxCollider>();
			std::vector<Pg::Data::StaticBoxCollider*> tStaticBoxCol = obj->GetComponents<Pg::Data::StaticBoxCollider>();
			std::vector<Pg::Data::SphereCollider*> tSphCol = obj->GetComponents<Pg::Data::SphereCollider>();
			std::vector<Pg::Data::CapsuleCollider*> tCapCol = obj->GetComponents<Pg::Data::CapsuleCollider>();
			std::vector<Pg::Data::PlaneCollider*> tPlaneCol = obj->GetComponents<Pg::Data::PlaneCollider>();

			//어떤 Collider인지에 따라 출력을 구분한다.
			if (!tBoxCol.empty())
			{
				MakeDynamicBoxCollider(obj);
			}
			else if (!tStaticBoxCol.empty())
			{
				MakeStaticBoxCollider(obj);
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
			AddObjectToScene();
		}
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

				physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(staticBoxcol->GetWidth() / 2,
					staticBoxcol->GetHeight() / 2, staticBoxcol->GetDepth() / 2), *_material);

				if (staticBoxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform localTm(physx::PxVec3(position.x, position.y, position.z));

				staticBoxcol->SetPxShape(boxShape);

				physx::PxRigidStatic* rigid = _physics->createRigidStatic(localTm);
				rigid->attachShape(*boxShape);

				staticBoxcol->SetPxRigidStatic(rigid);
				rigid->userData = staticBoxcol;
				_rigidStaticVec.push_back(rigid);

				boxShape->release();

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

				physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(boxcol->GetWidth() / 2,
					boxcol->GetHeight() / 2, boxcol->GetDepth() / 2), *_material);

				Pg::Math::PGFLOAT3 position = Pg::Math::PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());

				physx::PxTransform local(physx::PxVec3(position.x, position.y, position.z));

				//트리거를 위해 PXShape 지정.
				boxcol->SetPxShape(boxShape);

				if (boxcol->GetTrigger())
				{
					boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}


				//테스트를 위해 임시로 Rigid 넣어봄.
				//임시 아닌 이렇게 합쳐서 갈 예정.
				//2023.12.11
				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(local);

				//Rigid의 중력 조정
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(0.5f);
				rigid->attachShape(*boxShape);

				//_pxScene->addActor(*rigid);

				boxcol->SetPxRigidDynamic(rigid);
				rigid->userData = boxcol;
				_rigidDynamicVec.push_back(rigid);

				boxShape->release();

				///나중에 physicsSystem 사용 시
				//void* tRigid = rigid;
				//physx::PxRigidDynamic* tNewRigid = reinterpret_cast<physx::PxRigidDynamic*>(tRigid);

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
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(0, 0, 0.7071068f, 0.7071068f);
				shape->setLocalPose(trans);

				if (sphCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));

				sphCol->SetPxShape(shape);

				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(localTm);

				//Rigid의 중력 조정
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(0.5f);
				rigid->attachShape(*shape);
				//_pxScene->addActor(*rigid);

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

				Pg::Math::PGQuaternion quat = PGQuaternionMultiply(collider->GetRotationOffset(), obj->_transform._rotation);
				physx::PxTransform trans(physx::PxIdentity);
				trans.q = physx::PxQuat(0, 0, 0.7071068f, 0.7071068f);
				shape->setLocalPose(trans);

				if (capCol->GetTrigger())
				{
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}

				Pg::Math::PGFLOAT3 pos = PGFloat3MultiplyMatrix(collider->GetPositionOffset(), obj->_transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxIdentity);

				capCol->SetPxShape(shape);

				physx::PxRigidDynamic* rigid = _physics->createRigidDynamic(localTm);

				//Rigid의 중력 조정
				rigid->setAngularDamping(0.5f);
				rigid->setLinearDamping(0.5f);

				rigid->attachShape(*shape);

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
		/// ground 생성 후, 임의로 shape 붙여주기
		/*physx::PxRigidStatic* groundPlane = PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0), *_material);
		physx::PxShape* gpShape = _physics->createShape(physx::PxBoxGeometry(0.1f, 20.0f, 20.0f), *_material);
		groundPlane->attachShape(*gpShape);
		_pxScene->addActor(*groundPlane);*/

		Pg::Data::Collider* col = obj->GetComponent<Pg::Data::PlaneCollider>();

		if (col)
		{
			auto colliderVec = obj->GetComponents<Pg::Data::PlaneCollider>();

			for (auto& collider : colliderVec)
			{
				///PxPlane이 잘 안되는 것 같아 일단 박스처럼 생성.
				///추후에 PxPlane으로 출력할 예정.
				Pg::Data::PlaneCollider* planeCol = dynamic_cast<Pg::Data::PlaneCollider*>(collider);
				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(planeCol->GetWidth() / 2, 0.1f, planeCol->GetDepth() / 2), *_material);
				Pg::Math::PGFLOAT3 normal = planeCol->GetNormalVector();
				physx::PxTransform normalTm(physx::PxVec3(normal.x, normal.y, normal.z));
				//physx::PxPlane plane = { normal.x, normal.y, normal.z, planeCol->GetDistance() };

				planeCol->SetPxShape(shape);

				//physx::PxRigidStatic* rigid = PxCreatePlane(*_physics, plane, *_material);
				physx::PxRigidStatic* rigid = _physics->createRigidStatic(normalTm);
				rigid->attachShape(*shape);
				planeCol->SetPxRigidStatic(rigid);
				rigid->userData = planeCol;

				_rigidStaticVec.push_back(rigid);
			}
		}
	}


	//void PhysicSystem::MakeRayCast(Pg::Data::GameObject* obj)
	//{
	//	Pg::Data::RayCast* col = obj->GetComponent<Pg::Data::RayCast>();

	//	if (col)
	//	{
	//		auto rayCastVec = obj->GetComponents<Pg::Data::RayCast>();

	//		for (auto& rayCast : rayCastVec)
	//		{
	//			Pg::Data::RayCast* cRayCast = dynamic_cast<Pg::Data::RayCast*>(rayCast);

	//			//raycast의 크기.
	//			physx::PxVec3 rayCastOrigin;
	//			rayCastOrigin.x = cRayCast->GetOrigin().x;
	//			rayCastOrigin.y = cRayCast->GetOrigin().y;
	//			rayCastOrigin.z = cRayCast->GetOrigin().z;

	//			physx::PxVec3 rayCastDir;
	//			rayCastDir.x = cRayCast->GetDir().x;
	//			rayCastDir.y = cRayCast->GetDir().y;
	//			rayCastDir.z = cRayCast->GetDir().z;

	//			float length;
	//			length = cRayCast->GetLength();

	//			physx::PxRaycastBuffer _hitBuffer;
	//			_pxScene->raycast(rayCastOrigin, rayCastDir, length, _hitBuffer);
	//		}
	//	}
	//}


	///Rayscast 생성하기
	Pg::Data::Collider* PhysicSystem::MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, float tLength, int* bType)
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

		//RayCast 버퍼 생성.
		physx::PxRaycastBuffer _hitBuffer;
		bool _isHit = _pxScene->raycast(rayCastOrigin, rayCastDir, tLength, _hitBuffer);

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

			tHitPoint = _hitBuffer.block.position;
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

	///만들어진 Collider 객체를 Scene으로 추가하는 역할.
	void PhysicSystem::AddObjectToScene()
	{
		for (auto& rigidDynamic : _rigidDynamicVec)
		{
			static_cast<Pg::Data::DynamicCollider*>(rigidDynamic->userData)->UpdateTransform();
			_pxScene->addActor(*rigidDynamic);
		}

		for (auto& rigidStatic : _rigidStaticVec)
		{
			static_cast<Pg::Data::StaticCollider*>(rigidStatic->userData)->UpdateTransform();
			_pxScene->addActor(*rigidStatic);
		}
	}

	///매 프레임마다 충돌 이벤트 감지
	void PhysicSystem::Flush()
	{
		for (auto& rigid : _rigidDynamicVec)
		{
			static_cast<Pg::Data::DynamicCollider*>(rigid->userData)->Flush();
		}

		for (auto& rigid : _rigidStaticVec)
		{
			static_cast<Pg::Data::StaticCollider*>(rigid->userData)->Flush();
		}
	}

	void PhysicSystem::UpdateRayCast()
	{

	}

}