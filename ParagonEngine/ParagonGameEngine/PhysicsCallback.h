#pragma once
#include <PxPhysicsAPI.h>
#include <PxSimulationEventCallback.h>

#include <unordered_map>
#include <vector>

#include "PhysicsCollision.h"
#include "PhysicsColliderActor.h"

namespace Pg::Engine
{
	// Flax Engine을 모델링해서 만듬. 
	// https://flaxengine.com/

	class PhysicsCallback : public physx::PxSimulationEventCallback
	{
		//ColliderActor들의 Pair가 원래 되어야 한다. 
		using CollidersPair = std::pair<PhysicsColliderActor*, PhysicsColliderActor*>;
		using CollisionsPool = std::unordered_map<CollidersPair, PhysicsCollision>;

	public:

		//관리되고 있는 목록을 Clear한다.
		void Clear();

		// New / Old / Removed 콜리젼 생성, 알맞은 Trigger Pair 생성.
		void CollectResults();

		// Collision Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendCollisionEvents();

		// Trigger Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendTriggerEvents();

		//Collider가 제거되었을 때 호출됨. 캐시된 이벤트들은 해당 오브젝트에 대해 제거되어야 함.
		void OnColliderRemoved(PhysicsColliderActor* collider);

	public:
		//Collision들이 보관되는 Pool.
		CollisionsPool _collisions;

		//전 Step에서의 Collision들이 보관되는 Pool.
		CollisionsPool _prevCollisions;

		//Enter Event (OnCollisionEnter)를 위해 새롭게 생성된 Collision 세트들.
		std::vector<CollidersPair> _newCollisionPairs;

		//Exit Event (OnCollisionExit)를 위한 제거된 Collision 세트들.
		std::vector<CollidersPair> _removedCollisionPairs;

		//Enter Event (OnTriggerEnter)를 위해 새롭게 생성된 Trigger 세트들.
		std::vector<CollidersPair> _newTriggerPairs;

		//Exit Event (OnTriggerExit)를 위한 제거된 Trigger 세트들.
		std::vector<CollidersPair> _lostTriggerPairs;

	public:
		// [PxSimulationEventCallback] -> PhysX 자체적인 인터페이스를 오버라이드해서 사용한다.
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	private:
		//내부 헬퍼 함수들.
		void ClearColliderFromCollection(PhysicsColliderActor* collider, std::vector<PhysicsCallback::CollidersPair>& collection);
		void ClearColliderFromCollection(PhysicsColliderActor* collider, PhysicsCallback::CollisionsPool& collection);
	
	
	};
}

