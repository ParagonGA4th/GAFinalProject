#pragma once

//PhysX <-> CPP ������ ���� ���.
#include "PhysicsCallback.h"
#include "PhysicsAllocator.h"
#include "PhysicsErrorCallback.h"

#include "Pxphysics.h"
#include "PxphysicsAPI.h"
#include <PxSimulationEventCallback.h>
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonMath/PgMath.h"
#include <vector>
#include <memory>

#include "EngineDLLExporter.h"

/// <summary>
/// ParagonEngine�� ���� �ý��� Ŭ����.
/// PhysX �����Ͽ� ���������� ���� ���̴�
/// 2023.10.06
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class Collider;
}

namespace Pg::Engine
{
	class SceneSystem;
	class DebugSystem;
}

namespace Pg::Engine::Physic
{
	class PhysicSystem
	{
	public:
		void Initialize(Pg::Engine::DebugSystem* debugSystem);

		void ApplyRuntimeChangesCollider();

		void UpdatePhysics(float dTime);
		void UpdateTransform();

		void Finalize();

		void AddAllObjectsToScene();

		void Flush();

	public:
		//PhysX ����
		void CreatePxScene();

		void CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);	
	
	public:
		//Collider ��ü ����
		void InitMakeColliders();
		void CheckAddRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec);
		void CheckDeleteRuntimeColliders(const std::vector<Pg::Data::GameObject*>* vec);

		//StaticCollider ����
		void MakeStaticBoxCollider(Pg::Data::GameObject* obj);
		void MakeStaticCapsuleCollider(Pg::Data::GameObject* obj);
		void MakeStaticShpereCollider(Pg::Data::GameObject* obj);
		void MakePlaneCollider(Pg::Data::GameObject* obj);
		
		//DynamicCollider ����
		void MakeDynamicBoxCollider(Pg::Data::GameObject* obj);
		void MakeDynamicSphereCollider(Pg::Data::GameObject* obj);
		void MakeDynamicCapsuleCollider(Pg::Data::GameObject* obj);
		
		//RayCast ����
		//PARAGON_ENGINE_DLL Pg::Data::Collider* MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, 
		//	float tLength, int* bType);
		//��Ʈ�� ����Ʈ�� �����ڷ� �����ϰ� ����, Collider�� Nullptr�� �ƴ� ��� ������ ���ϰ��� ���ο��� ����� ���̴�.
		PARAGON_ENGINE_DLL Pg::Data::Collider* MakeRayCast(Pg::Math::PGFLOAT3 tOrigin, Pg::Math::PGFLOAT3 tDir, float tLength, Pg::Math::PGFLOAT3& outHitPoint, int* bType, bool detectTriggers);
		PARAGON_ENGINE_DLL void MakeSphereCast(const Pg::Math::PGFLOAT3& tOrigin, const Pg::Math::PGFLOAT3& tDir, 
			float tRad, float max, unsigned int maxColCnt, Pg::Data::Collider** colDataPointer);

	private:
		void ApplyAddSingleCollider(Pg::Data::GameObject* obj);
		void ApplyDeleteSingleCollider(Pg::Data::GameObject* obj);

	private:
		//Rigid ������ ��Ƴ��� ����
		std::vector<physx::PxRigidDynamic*> _rigidDynamicVec;
		std::vector<physx::PxRigidStatic*> _rigidStaticVec;

	private:
		
		//PhysX������ ���� ������
		//�޸� : ��ü ������� Allocate�Ѵ�. From Flax.
		//physx::PxDefaultAllocator		_allocator;
		PhysicsAllocator				_allocator;

		//������, PG_WARN �������� ����� ��.
		//physx::PxDefaultErrorCallback	_errorCallback;
		PhysicsErrorCallback			_errorCallback;
		physx::PxTolerancesScale		_toleranceScale;
		physx::PxPhysics*				_physics;
		physx::PxFoundation*			_foundation = nullptr;
		physx::PxDefaultCpuDispatcher*	_dispatcher = nullptr;
		physx::PxScene*					_pxScene = nullptr;
		physx::PxMaterial*				_material = nullptr;
		physx::PxPvd*					_pvd = nullptr;

		Pg::Engine::SceneSystem* _sceneSystem;
		Pg::Engine::DebugSystem* _debugSystem = nullptr;

		std::unique_ptr<PhysicsCallback> _physicsCallback;

		std::unique_ptr<Pg::Data::SphereInfo> _forSweepSphereInfo{ nullptr };

		std::vector<Pg::Data::GameObject*> _tempAddedObjectsInPhysics;
		std::vector<Pg::Data::GameObject*> _tempDeletedObjectsInPhysics;

	private:
		const float fixedDeltaTime = 1.0f / 60.0f;
		float accumulator = 0.f;

	private:
		bool _isScene3D{ true };
	};
}

