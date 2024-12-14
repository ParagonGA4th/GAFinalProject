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
	// Flax Engine�� �𵨸��ؼ� ����. 

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

//CollidersPair�� Unordered_map�� Key�� ���� ���� std �����ε�.
//Ŀ���� Ŭ������ Key�� ����Ϸ���, �ش� Ŭ�������� ���� hash / equal_to�� ���ø� Ư��ȭ����� �Ѵ�.
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
		//�����ǰ� �ִ� ����� Clear�Ѵ�.
		void Clear();

		// New / Old / Removed �ݸ��� ����, �˸��� Trigger Pair ����.
		void CollectResults();

		// Collision Event���� �����ǰ� �ִ� ������Ʈ�鿡�� ������.
		void SendCollisionEvents();

		// Trigger Event���� �����ǰ� �ִ� ������Ʈ�鿡�� ������.
		void SendTriggerEvents();

		//Collider�� ���ŵǾ��� �� ȣ���. ĳ�õ� �̺�Ʈ���� �ش� ������Ʈ�� ���� ���ŵǾ�� ��.
		void OnColliderRemoved(Pg::Data::Collider* collider);

	public:
		//Collision���� �����Ǵ� Pool.
		CollisionsPool _collisions;

		//�� Step������ Collision���� �����Ǵ� Pool.
		CollisionsPool _prevCollisions;

		//Enter Event (OnCollisionEnter)�� ���� ���Ӱ� ������ Collision ��Ʈ��.
		std::vector<CollidersPair> _newCollisionPairs;

		//Exit Event (OnCollisionExit)�� ���� ���ŵ� Collision ��Ʈ��.
		std::vector<CollidersPair> _removedCollisionPairs;

		//Enter Event (OnTriggerEnter)�� ���� ���Ӱ� ������ Trigger ��Ʈ��.
		std::vector<CollidersPair> _newTriggerPairs;

		//Exit Event (OnTriggerExit)�� ���� ���ŵ� Trigger ��Ʈ��.
		std::vector<CollidersPair> _lostTriggerPairs;

	public:
		// [PxSimulationEventCallback] -> PhysX ��ü���� �������̽��� �������̵��ؼ� ����Ѵ�.
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	private:
		//���� ���� �Լ���.
		void ClearColliderFromCollection(Pg::Data::Collider* collider, std::vector<CollidersPair>& collection);
		void ClearColliderFromCollection(Pg::Data::Collider* collider, CollisionsPool& collection);
	
	
	};
}

