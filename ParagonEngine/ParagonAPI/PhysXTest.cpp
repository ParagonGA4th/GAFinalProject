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
}