#pragma once
#include <PxPhysicsAPI.h>
#include <PxSimulationEventCallback.h>

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/Collider.h"

namespace Pg::Engine
{
	// Flax Engine을 모델링해서 만듬. 

	class CollidersPair
	{
	public:
		CollidersPair() = default;
		CollidersPair(Pg::Data::Collider* first, Pg::Data::Collider* second) :
			_first(first), _second(second)
		{}

		Pg::Data::Collider* _first;
		Pg::Data::Collider* _second;
	};
}

//CollidersPair를 Unordered_map의 Key로 쓰기 위해 std 오버로드.
//커스텀 클래스를 Key로 사용하려면, 해당 클래스에서 사용될 hash / equal_to를 템플릿 특수화해줘야 한다.
namespace std {
	template <>
	struct hash<Pg::Engine::CollidersPair> {
		size_t operator()(const Pg::Engine::CollidersPair& t) const 
		{
			//return (((size_t)t.a) << 16) + (((size_t)t.b) << 8) + ((size_t)t.c);
			return std::hash<Pg::Data::Collider*>()(t._first) ^ std::hash<Pg::Data::Collider*>()(t._second);
		}
	};
	template <>
	struct equal_to<Pg::Engine::CollidersPair> {
		bool operator()(const Pg::Engine::CollidersPair& lhs, const Pg::Engine::CollidersPair& rhs) const 
		{
			return (lhs._first == rhs._first) && (lhs._second == rhs._second);
		}
	};
}

namespace Pg::Engine
{
	class PhysicsCallback : public physx::PxSimulationEventCallback
	{
	public:
		PhysicsCallback();
		~PhysicsCallback();

		using CollisionsPool = std::unordered_map<CollidersPair, Pg::Data::PhysicsCollision>;
		//관리되고 있는 목록을 Clear한다.
		void Clear();

		// New / Old / Removed 콜리젼 생성, 알맞은 Trigger Pair 생성.
		void CollectResults();

		// Collision Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendCollisionEvents();

		// Trigger Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendTriggerEvents();

		//Collider가 제거되었을 때 호출됨. 캐시된 이벤트들은 해당 오브젝트에 대해 제거되어야 함.
		void OnColliderRemoved(Pg::Data::Collider* collider);

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
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	private:
		//내부 헬퍼 함수들.
		void ClearColliderFromCollection(Pg::Data::Collider* collider, std::vector<CollidersPair>& collection);
		void ClearColliderFromCollection(Pg::Data::Collider* collider, CollisionsPool& collection);
	
	
	};
}

