#include "PhysicsCallback.h"
#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <cassert>

namespace Pg::Engine
{
	PhysicsCallback::PhysicsCallback()
	{
		//
	}

	PhysicsCallback::~PhysicsCallback()
	{
		//
	}

	void PhysicsCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		// 안 씀.
	}

	void PhysicsCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		// 안 씀.
	}

	void PhysicsCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		// 안 씀.
	}

	void PhysicsCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{;
		//PG_TRACE("On Contact");
		using namespace physx;

		// 제거된 액터들에게 이벤트 보내는 것은 그만.
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		{
			return;
		}

		//Collision 하나가 발생할 때마다 생성되는 객체.
		Pg::Data::PhysicsCollision c;
		c._thisVelocity = { 0.f,0.f,0.f };
		c._otherVelocity = { 0.f,0.f,0.f };

		// Collision Pair를 빼내기.
		for (PxU32 pairIndex = 0; pairIndex < nbPairs; pairIndex++)
		{
			//충돌된 ContactPair를 받는다.
			const PxContactPair& pair = pairs[pairIndex];

			//압축된 Contact들의 압축된 스트림을 Read-Only로 접근할 수 있게 해준다.
			PxContactStreamIterator i(pair.contactPatches, pair.contactPoints, pair.getInternalFaceIndices(), pair.patchCount, pair.contactCount);

			//Impulse 정보를 가져오기.
			const PxReal* impulses = pair.contactImpulses;
			const PxU32 flippedContacts = (pair.flags & PxContactPairFlag::eINTERNAL_CONTACTS_ARE_FLIPPED);

			//Impulse가 내부적으로 존재하는지 확인.
			const PxU32 hasImpulses = (pair.flags & PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
			PxU32 nbContacts = 0;
			PxVec3 totalImpulse = PxVec3(0.0f);

			//부딪힌 This / Other 액터 포인터 옮기기.
			c._thisActor = static_cast<Pg::Data::Collider*>(pair.shapes[0]->userData);
			c._otherActor = static_cast<Pg::Data::Collider*>(pair.shapes[1]->userData);

			//둘 다 유효한지 확인.
			assert(c._thisActor != nullptr && c._otherActor != nullptr);

			//스트림 내부에 더 읽어들일 수 있는 패치가 있을 때까지.
			while (i.hasNextPatch())
			{
				//다음 Patch로 Iterator++
				i.nextPatch();

				//다음 Contact가 있고, 감지할 수 있는 Contact의 포인트보다 작을 때 
				while (i.hasNextContact() && nbContacts < Pg::Data::PhysicsCollision::MAX_CONTACT_POINTS)
				{
					//다음 Contact로 Iterator++
					i.nextContact();

					//Impulse 정보 받기.
					const PxVec3 point = i.getContactPoint();
					const PxVec3 normal = i.getContactNormal();

					//TotalImpulse 기록하기. 
					if (hasImpulses)
					{
						totalImpulse += normal * impulses[nbContacts];
					}

					//[NOTUSED] PxU32 internalFaceIndex0 = flippedContacts ? iter.getFaceIndex1() : iter.getFaceIndex0();
					//[NOTUSED] PxU32 internalFaceIndex1 = flippedContacts ? iter.getFaceIndex0() : iter.getFaceIndex1();

					//Collision의 Contact Point를 제어하기 위해.
					Pg::Data::PhysicsContactPoint& contact = c._contacts[nbContacts];
					contact._point = { point.x, point.y, point.z };
					contact._normal = { normal.x, normal.y, normal.z };
					contact._separation = i.getSeparation();

					//Contact들의 개수를 + 1.
					nbContacts++;
				}
			}

			//PhysX 연동 추가 기록.
			c._activeContactCount = nbContacts;
			c._impulse = { totalImpulse.x, totalImpulse.y, totalImpulse.z };


			//Collision 목록 기록. (Callback 클래스에서)
			_collisions[CollidersPair(c._thisActor, c._otherActor)] = c;
		}

		//Velocity를 빼내기.
		PxContactPairExtraDataIterator i(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);

		//다음 아이템 세트가 있을 때까지
		while (i.nextItemSet())
		{
			//다음 Post Solver Velocity가 있을 경우에
			if (i.postSolverVelocity)
			{
				const PxVec3 linearVelocityActor0 = i.postSolverVelocity->linearVelocity[0];
				const PxVec3 linearVelocityActor1 = i.postSolverVelocity->linearVelocity[1];

				//ContactPair에 접속.
				const PxContactPair& pair = pairs[i.contactPairIndex];

				//콜라이더 Actor를 가져오는 과정이다.
				c._thisActor = static_cast<Pg::Data::Collider*>(pair.shapes[0]->userData);
				c._otherActor = static_cast<Pg::Data::Collider*>(pair.shapes[1]->userData);

				Pg::Data::PhysicsCollision& collision = _collisions[CollidersPair(c._thisActor, c._otherActor)];
				//둘 다 유효한지 확인.
				assert(c._thisActor != nullptr && c._otherActor != nullptr);

				collision._thisVelocity = { linearVelocityActor0.x, linearVelocityActor0.y, linearVelocityActor0.z };
				collision._otherVelocity = { linearVelocityActor1.x, linearVelocityActor1.y, linearVelocityActor1.z };
			}
		}
	}

	void PhysicsCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		//PG_TRACE("On Trigger");

		using namespace physx;
		
		for (PxU32 i = 0; i < count; i++)
		{
			//Trigger Pair를 받아오기.
			const PxTriggerPair& pair = pairs[i];
		
			// 지워진 Shape들의 Trigger들은 무시하기.
			if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;
		
			//Trigger들의 주소를 받기.
			
			Pg::Data::Collider* trigger = static_cast<Pg::Data::Collider*>(pair.triggerShape->userData);
			Pg::Data::Collider* otherCollider = static_cast<Pg::Data::Collider*>(pair.otherShape->userData);
		
			//둘 다 제대로 존재하는지 확인하기.
			assert(trigger != nullptr && otherCollider != nullptr);
		
			//Collider들의 Pair 만들기.
			CollidersPair collidersPair(trigger, otherCollider);
		
			//상태를 확인했는데, 만약 이제 Trigger를 읽어버렸다면?
			if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//잃어버렸다고 표시.
				_lostTriggerPairs.emplace_back(collidersPair);
			}
			else
			{
				//새롭게 추가된 Trigger 목록에 추가.
				_newTriggerPairs.emplace_back(collidersPair);
			}
		}
	}

	void PhysicsCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		// 안 씀.
	}

	void PhysicsCallback::Clear()
	{
		//서로를 바꿔주고, Clear.
		std::swap(_collisions, _prevCollisions);
		_collisions.clear();

		_newCollisionPairs.clear();
		_removedCollisionPairs.clear();

		_newTriggerPairs.clear();
		_lostTriggerPairs.clear();
	}

	void PhysicsCallback::ClearColliderFromCollection(Pg::Data::Collider* collider, std::vector<CollidersPair>& collection)
	{
		//C++ Erase-Remove Idiom
		//특정 조건 충족 요건 제거
		collection.erase(std::remove_if(collection.begin(),
			collection.end(),
			[&collider](const CollidersPair& colPair) -> bool
			{ return (colPair._first == collider || colPair._second == collider); }),
			collection.end());
	}

	void PhysicsCallback::ClearColliderFromCollection(Pg::Data::Collider* collider, CollisionsPool& collection)
	{
		//C++ Erase-Remove Idiom
		//특정 조건 충족 요건 제거
		//다만, Vector에는 충족했지만, std::unordered_map은 지원하지 않았음.

		//C++20부터, erase_if를 std::unordered_map에도 지원.
		const auto count = std::erase_if(collection, [&collider](const auto& item) {
			auto const& [key, value] = item;
			return (key._first == collider || key._second == collider);
			});
	}

	void PhysicsCallback::CollectResults()
	{
		//빌드 안되는 문제.
		for (auto& it : _collisions)
		{
			//전 목록에 Collision이 없는데 현재에는 있으면
			if (!_prevCollisions.contains(it.first))
			{
				//Collision 생성.
				_newCollisionPairs.push_back(it.first);
			}
		}

		for (auto& it : _prevCollisions)
		{
			//전 목록에 Collision이 없는데 현재에는 있으면
			if (!_collisions.contains(it.first))
			{
				//Collision 생성.
				_removedCollisionPairs.push_back(it.first);
			}
		}
	}

	void PhysicsCallback::SendCollisionEvents()
	{
		//제거된 Collision Pair에서
		for (int i = 0; i < _removedCollisionPairs.size(); i++)
		{
			//막 제거된 CollisionPair.
			const auto& pair = _removedCollisionPairs[i];

			//해당 인덱스된 PhysicsCollision을 가져온다.
			auto& c = _prevCollisions[pair];

			//OnCollisionExit 함수들 발동.
			//pair._first->Collider_OnCollisionExit(c);
			//c.SwapObjects();
			//pair._second->Collider_OnCollisionExit(c);
			//c.SwapObjects();

			//OnCollisionExit 함수들 발동.
			if (!pair._first->GetTrigger())
			{
				pair._first->Collider_OnCollisionExit(c);
			}
			c.SwapObjects();
			if (!pair._second->GetTrigger())
			{
				pair._second->Collider_OnCollisionExit(c);
			}
			c.SwapObjects();
		}

		for (int i = 0; i < _newCollisionPairs.size(); i++)
		{
			//막 추가된 CollisionPair.
			const auto& pair = _newCollisionPairs[i];

			//해당 인덱스된 PhysicsCollision을 가져온다.
			auto& c = _collisions[pair];

			//OnCollisionEnter 함수를 발동.
			//OnCollisionExit 함수들 발동.
			if (!pair._first->GetTrigger())
			{
				pair._first->Collider_OnCollisionEnter(c);
			}
			c.SwapObjects();
			if (!pair._second->GetTrigger())
			{
				pair._second->Collider_OnCollisionEnter(c);
			}
			c.SwapObjects();
		}
	}

	void PhysicsCallback::SendTriggerEvents()
	{
		for (int i = 0; i < _lostTriggerPairs.size(); i++)
		{
			const auto& c = _lostTriggerPairs[i];

			//서로의 함수를 호출. (OnTriggerExit)
			if (c._first->GetTrigger())
			{
				c._first->Collider_OnTriggerExit(c._second);
			}
			if (c._second->GetTrigger())
			{
				c._second->Collider_OnTriggerExit(c._first);
			}
		}

		for (int i = 0; i < _newTriggerPairs.size(); i++)
		{
			//서로의 함수를 호출. (OnTriggerEnter)
			const auto& c = _newTriggerPairs[i];

			if (c._first->GetTrigger())
			{
				c._first->Collider_OnTriggerEnter(c._second);
			}
			if (c._second->GetTrigger())
			{
				c._second->Collider_OnTriggerEnter(c._first);
			}
		}
	}

	void PhysicsCallback::OnColliderRemoved(Pg::Data::Collider* collider)
	{
		ClearColliderFromCollection(collider, _collisions);
		ClearColliderFromCollection(collider, _prevCollisions);
		ClearColliderFromCollection(collider, _removedCollisionPairs);
		ClearColliderFromCollection(collider, _newTriggerPairs);
		ClearColliderFromCollection(collider, _lostTriggerPairs);
	}



}