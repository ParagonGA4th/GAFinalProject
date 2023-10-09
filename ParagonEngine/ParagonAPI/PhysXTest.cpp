//#include "PhysXTest.h"
//#include "../lib/PhysX/include/PxPhysics.h"
//#include "../lib/PhysX/include/PxPhysicsAPI.h"
//#include <iostream>
//#include <windows.h>
//
//namespace Pg::API
//{
//	void PhysXTest::Initialize()
//	{
//		using namespace physx;
//
//		physx::PxDefaultAllocator mDefaultAllocatorCallback;
//		physx::PxDefaultErrorCallback mDefaultErrorCallback;
//
//		physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
//
//		physx::PxTolerancesScale mToleranceScale;
//
//		physx::PxFoundation* mFoundation = NULL;
//
//		physx::PxPhysics* mPhysics = NULL;
//
//		physx::PxScene* mScene = NULL;
//		physx::PxMaterial* mMaterial = NULL;
//
//		physx::PxPvd* mPvd = NULL;
//
//		///여기서부터 초기화 시작!
//		// foundation 세팅 및 체크.
//		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
//		if (!mFoundation) throw("PxCreateFoundation failed!");
//
//		//PhysX의 디버그 정보를 연결한다. 이때 자신의 IP를 매개변수로 받아와야 함.
//		mPvd = PxCreatePvd(*mFoundation);
//		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.126.1.161", 5425, 10);
//		mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
//
//		mToleranceScale.length = 100;        // typical length of an object
//		mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
//
//		//본격적으로 PhysX 생성
//		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
//
//		//아마 PhysX의 Scene에 대한 설정 같아보인다.
//		physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
//		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
//		sceneDesc.cpuDispatcher = mDispatcher;
//		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
//
//		mScene = mPhysics->createScene(sceneDesc);
//
//		//Pvd의 Scene과 Client를 연결하는 작업인 것 같다.
//		physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
//		if (pvdClient)
//		{
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//		}
//
//		// 바닥
//		mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//		physx::PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 0), *mMaterial);
//		physx::PxShape* gpShape = mPhysics->createShape(physx::PxBoxGeometry(1.0f, 256.0f, 256.0f), *mMaterial);
//		groundPlane->attachShape(*gpShape);
//		mScene->addActor(*groundPlane);
//
//		//박스 무더기 생성
//		float halfExtent = 1.0f;
//		physx::PxShape* shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
//		physx::PxU32 size = 30;
//		physx::PxTransform t(physx::PxVec3(0));
//
//		//캡슐
//		PxRigidDynamic* _capsuleObject = mPhysics->createRigidDynamic(PxTransform(1.0f, 1.0f, 1.0f));
//		PxTransform relativePos(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
//		physx::PxCapsuleGeometry _capsule;
//		physx::PxShape* _shape = mPhysics->createShape(_capsule, *mMaterial);
//	}
//}
