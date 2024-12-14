#include "PhysicsCollision.h"
#include "Collider.h"
#include <algorithm>

namespace Pg::Data
{
	void PhysicsCollision::SwapObjects()
	{
		std::swap(_thisActor, _otherActor);
		std::swap(_thisVelocity, _otherVelocity);
	}

	Pg::Data::Collider* PhysicsCollision::GetActualOtherActor(Pg::Data::PhysicsCollision* phyCol, Pg::Data::GameObject* selfObj)
	{
		Pg::Data::Collider* tRealOtherActor = nullptr;

		if (phyCol->_thisActor->_object != selfObj)
		{
			tRealOtherActor = phyCol->_thisActor;
		}
		else
		{
			tRealOtherActor = phyCol->_otherActor;
		}

		return tRealOtherActor;
	}

}