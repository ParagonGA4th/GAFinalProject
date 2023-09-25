#include <../PhysX/physx/installed/x64-uwp/include/PxPhysicsAPI.h>
#include <../PhysX/physx/installed/x64-uwp/include/PxPhysics.h>
#include <iostream>
#include <windows.h>

int main()
{
	// SDK가 필요로 하는 allocator 인터페이스에 대한 implementation.
	physx::PxDefaultAllocator		mDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback	mDefaultErrorCallback;

	// cpu, 멀티쓰레딩 관련.
	physx::PxDefaultCpuDispatcher* mDispatcher = NULL;

	// 길이, 속도, 질량, 중력 등의 오차를 보정할 때 사용하는 값.
	physx::PxTolerancesScale		mToleranceScale;

	// 세팅.
	physx::PxFoundation* mFoundation = NULL;

	// 오브젝트를 인스턴스화 하는 팩토리 클래스이자, 코어부분.
	physx::PxPhysics* mPhysics = NULL;


	// 시뮬레이션 수행하는 씬, 표면 재질에 관한 material
	physx::PxScene* mScene = NULL;
	physx::PxMaterial* mMaterial = NULL;
}