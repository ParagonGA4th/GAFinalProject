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
		// �� ��.
	}

	void PhysicsCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		// �� ��.
	}

	void PhysicsCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		// �� ��.
	}

	void PhysicsCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{;
		//PG_TRACE("On Contact");
		using namespace physx;

		// ���ŵ� ���͵鿡�� �̺�Ʈ ������ ���� �׸�.
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		{
			return;
		}

		//Collision �ϳ��� �߻��� ������ �����Ǵ� ��ü.
		Pg::Data::PhysicsCollision c;
		c._thisVelocity = { 0.f,0.f,0.f };
		c._otherVelocity = { 0.f,0.f,0.f };

		// Collision Pair�� ������.
		for (PxU32 pairIndex = 0; pairIndex < nbPairs; pairIndex++)
		{
			//�浹�� ContactPair�� �޴´�.
			const PxContactPair& pair = pairs[pairIndex];

			//����� Contact���� ����� ��Ʈ���� Read-Only�� ������ �� �ְ� ���ش�.
			PxContactStreamIterator i(pair.contactPatches, pair.contactPoints, pair.getInternalFaceIndices(), pair.patchCount, pair.contactCount);

			//Impulse ������ ��������.
			const PxReal* impulses = pair.contactImpulses;
			const PxU32 flippedContacts = (pair.flags & PxContactPairFlag::eINTERNAL_CONTACTS_ARE_FLIPPED);

			//Impulse�� ���������� �����ϴ��� Ȯ��.
			const PxU32 hasImpulses = (pair.flags & PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
			PxU32 nbContacts = 0;
			PxVec3 totalImpulse = PxVec3(0.0f);

			//�ε��� This / Other ���� ������ �ű��.
			c._thisActor = static_cast<Pg::Data::Collider*>(pair.shapes[0]->userData);
			c._otherActor = static_cast<Pg::Data::Collider*>(pair.shapes[1]->userData);

			//�� �� ��ȿ���� Ȯ��.
			assert(c._thisActor != nullptr && c._otherActor != nullptr);

			//��Ʈ�� ���ο� �� �о���� �� �ִ� ��ġ�� ���� ������.
			while (i.hasNextPatch())
			{
				//���� Patch�� Iterator++
				i.nextPatch();

				//���� Contact�� �ְ�, ������ �� �ִ� Contact�� ����Ʈ���� ���� �� 
				while (i.hasNextContact() && nbContacts < Pg::Data::PhysicsCollision::MAX_CONTACT_POINTS)
				{
					//���� Contact�� Iterator++
					i.nextContact();

					//Impulse ���� �ޱ�.
					const PxVec3 point = i.getContactPoint();
					const PxVec3 normal = i.getContactNormal();

					//TotalImpulse ����ϱ�. 
					if (hasImpulses)
					{
						totalImpulse += normal * impulses[nbContacts];
					}

					//[NOTUSED] PxU32 internalFaceIndex0 = flippedContacts ? iter.getFaceIndex1() : iter.getFaceIndex0();
					//[NOTUSED] PxU32 internalFaceIndex1 = flippedContacts ? iter.getFaceIndex0() : iter.getFaceIndex1();

					//Collision�� Contact Point�� �����ϱ� ����.
					Pg::Data::PhysicsContactPoint& contact = c._contacts[nbContacts];
					contact._point = { point.x, point.y, point.z };
					contact._normal = { normal.x, normal.y, normal.z };
					contact._separation = i.getSeparation();

					//Contact���� ������ + 1.
					nbContacts++;
				}
			}

			//PhysX ���� �߰� ���.
			c._activeContactCount = nbContacts;
			c._impulse = { totalImpulse.x, totalImpulse.y, totalImpulse.z };


			//Collision ��� ���. (Callback Ŭ��������)
			_collisions[CollidersPair(c._thisActor, c._otherActor)] = c;
		}

		//Velocity�� ������.
		PxContactPairExtraDataIterator i(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);

		//���� ������ ��Ʈ�� ���� ������
		while (i.nextItemSet())
		{
			//���� Post Solver Velocity�� ���� ��쿡
			if (i.postSolverVelocity)
			{
				const PxVec3 linearVelocityActor0 = i.postSolverVelocity->linearVelocity[0];
				const PxVec3 linearVelocityActor1 = i.postSolverVelocity->linearVelocity[1];

				//ContactPair�� ����.
				const PxContactPair& pair = pairs[i.contactPairIndex];

				//�ݶ��̴� Actor�� �������� �����̴�.
				c._thisActor = static_cast<Pg::Data::Collider*>(pair.shapes[0]->userData);
				c._otherActor = static_cast<Pg::Data::Collider*>(pair.shapes[1]->userData);

				Pg::Data::PhysicsCollision& collision = _collisions[CollidersPair(c._thisActor, c._otherActor)];
				//�� �� ��ȿ���� Ȯ��.
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
			//Trigger Pair�� �޾ƿ���.
			const PxTriggerPair& pair = pairs[i];
		
			// ������ Shape���� Trigger���� �����ϱ�.
			if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;
		
			//Trigger���� �ּҸ� �ޱ�.
			
			Pg::Data::Collider* trigger = static_cast<Pg::Data::Collider*>(pair.triggerShape->userData);
			Pg::Data::Collider* otherCollider = static_cast<Pg::Data::Collider*>(pair.otherShape->userData);
		
			//�� �� ����� �����ϴ��� Ȯ���ϱ�.
			assert(trigger != nullptr && otherCollider != nullptr);
		
			//Collider���� Pair �����.
			CollidersPair collidersPair(trigger, otherCollider);
		
			//���¸� Ȯ���ߴµ�, ���� ���� Trigger�� �о���ȴٸ�?
			if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//�Ҿ���ȴٰ� ǥ��.
				_lostTriggerPairs.emplace_back(collidersPair);
			}
			else
			{
				//���Ӱ� �߰��� Trigger ��Ͽ� �߰�.
				_newTriggerPairs.emplace_back(collidersPair);
			}
		}
	}

	void PhysicsCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		// �� ��.
	}

	void PhysicsCallback::Clear()
	{
		//���θ� �ٲ��ְ�, Clear.
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
		//Ư�� ���� ���� ��� ����
		collection.erase(std::remove_if(collection.begin(),
			collection.end(),
			[&collider](const CollidersPair& colPair) -> bool
			{ return (colPair._first == collider || colPair._second == collider); }),
			collection.end());
	}

	void PhysicsCallback::ClearColliderFromCollection(Pg::Data::Collider* collider, CollisionsPool& collection)
	{
		//C++ Erase-Remove Idiom
		//Ư�� ���� ���� ��� ����
		//�ٸ�, Vector���� ����������, std::unordered_map�� �������� �ʾ���.

		//C++20����, erase_if�� std::unordered_map���� ����.
		const auto count = std::erase_if(collection, [&collider](const auto& item) {
			auto const& [key, value] = item;
			return (key._first == collider || key._second == collider);
			});
	}

	void PhysicsCallback::CollectResults()
	{
		//���� �ȵǴ� ����.
		for (auto& it : _collisions)
		{
			//�� ��Ͽ� Collision�� ���µ� ���翡�� ������
			if (!_prevCollisions.contains(it.first))
			{
				//Collision ����.
				_newCollisionPairs.push_back(it.first);
			}
		}

		for (auto& it : _prevCollisions)
		{
			//�� ��Ͽ� Collision�� ���µ� ���翡�� ������
			if (!_collisions.contains(it.first))
			{
				//Collision ����.
				_removedCollisionPairs.push_back(it.first);
			}
		}
	}

	void PhysicsCallback::SendCollisionEvents()
	{
		//���ŵ� Collision Pair����
		for (int i = 0; i < _removedCollisionPairs.size(); i++)
		{
			//�� ���ŵ� CollisionPair.
			const auto& pair = _removedCollisionPairs[i];

			//�ش� �ε����� PhysicsCollision�� �����´�.
			auto& c = _prevCollisions[pair];

			//OnCollisionExit �Լ��� �ߵ�.
			//pair._first->Collider_OnCollisionExit(c);
			//c.SwapObjects();
			//pair._second->Collider_OnCollisionExit(c);
			//c.SwapObjects();

			//OnCollisionExit �Լ��� �ߵ�.
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
			//�� �߰��� CollisionPair.
			const auto& pair = _newCollisionPairs[i];

			//�ش� �ε����� PhysicsCollision�� �����´�.
			auto& c = _collisions[pair];

			//OnCollisionEnter �Լ��� �ߵ�.
			//OnCollisionExit �Լ��� �ߵ�.
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

			//������ �Լ��� ȣ��. (OnTriggerExit)
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
			//������ �Լ��� ȣ��. (OnTriggerEnter)
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