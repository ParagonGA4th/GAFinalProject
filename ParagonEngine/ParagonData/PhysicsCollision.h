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
		///PhysicsCollision이 리턴될 때, thisActor, otherActor의 순서가 일정하지 않다.
		///그러니, 실제로 다른 충돌한 Actor를 사용!
		static Pg::Data::Collider* GetActualOtherActor(Pg::Data::PhysicsCollision* phyCol, Pg::Data::GameObject* selfObj);


	public:
		enum {MAX_CONTACT_POINTS = 8};
	public:
		//오브젝트를 서로 바꾸기.
		void SwapObjects();
		
		//Contact Pair에 가해진 전체 Impulse (콜리젼을 반영하기 위해)
		Pg::Math::PGFLOAT3 _impulse;

		// (This) 부딪힌 오브젝트의 선형적 Velocity.
		Pg::Math::PGFLOAT3 _thisVelocity;

		// (Other) 부딪힌 오브젝트의 선형적 Velocity.
		Pg::Math::PGFLOAT3 _otherVelocity;

		//유효한 Contact Point들의 숫자. (MAX_CONTACT_POINTS보다 적어야 함.)
		int _contactsCount;

		// (This) 충돌한 Actor.
		Collider* _thisActor = nullptr;

		// (Other) 충돌한 Actor.
		Collider* _otherActor = nullptr;

		//Contact Point들.
		PhysicsContactPoint _contacts[MAX_CONTACT_POINTS];

		//실제로 유효한 Contact Point들의 수를 기록한다.
		unsigned int _activeContactCount = 0;
	};
}

//