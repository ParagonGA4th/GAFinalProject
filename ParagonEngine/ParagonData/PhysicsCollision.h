#pragma once
#include "../ParagonMath/PgMath.h"
#include "PhysicsContactPoint.h"

namespace Pg::Data
{
	class Collider;
	class GameObject;
}

namespace Pg::Data
{
	class PhysicsCollision
	{
	public:
		///PhysicsCollision�� ���ϵ� ��, thisActor, otherActor�� ������ �������� �ʴ�.
		///�׷���, ������ �ٸ� �浹�� Actor�� ���!
		static Pg::Data::Collider* GetActualOtherActor(Pg::Data::PhysicsCollision* phyCol, Pg::Data::GameObject* selfObj);


	public:
		enum {MAX_CONTACT_POINTS = 8};
	public:
		//������Ʈ�� ���� �ٲٱ�.
		void SwapObjects();
		
		//Contact Pair�� ������ ��ü Impulse (�ݸ����� �ݿ��ϱ� ����)
		Pg::Math::PGFLOAT3 _impulse;

		// (This) �ε��� ������Ʈ�� ������ Velocity.
		Pg::Math::PGFLOAT3 _thisVelocity;

		// (Other) �ε��� ������Ʈ�� ������ Velocity.
		Pg::Math::PGFLOAT3 _otherVelocity;

		//��ȿ�� Contact Point���� ����. (MAX_CONTACT_POINTS���� ����� ��.)
		int _contactsCount;

		// (This) �浹�� Actor.
		Collider* _thisActor = nullptr;

		// (Other) �浹�� Actor.
		Collider* _otherActor = nullptr;

		//Contact Point��.
		PhysicsContactPoint _contacts[MAX_CONTACT_POINTS];

		//������ ��ȿ�� Contact Point���� ���� ����Ѵ�.
		unsigned int _activeContactCount = 0;
	};
}

//