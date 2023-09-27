#include "../lib/PhysX/physx/installed/x64-windows/include/PxPhysics.h"
#include "../lib/PhysX/physx/installed/x64-windows/include/PxPhysicsAPI.h"
#include <windows.h>
#include <iostream>

void main()
{
	physx::PxDefaultAllocator mDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback mDefaultErrorCallback;

	physx::PxDefaultCpuDispatcher* mDispatcher = NULL;

	physx::PxTolerancesScale mToleranceScale;

	physx::PxFoundation* mFoundation = NULL;

	physx::PxPhysics* mPhysics = NULL;

	physx::PxScene* mScene = NULL;
	physx::PxMaterial* mMaterial = NULL;

	physx::PxPvd* mPvd = NULL;

	///여기서부터 초기화 시작!
	// foundation 세팅 및 체크.
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
	if (!mFoundation) throw("PxCreateFoundation failed!");

	mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	mToleranceScale.length = 100;        // typical length of an object
	mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

	//본격적으로 PhysX 생성
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);

	//아마 PhysX의 Scene에 대한 설정 같아보인다.
	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	mScene = mPhysics->createScene(sceneDesc);
}