#include "PhysicsCollision.h"
#include "PhysicsColliderActor.h"
#include <algorithm>

namespace Pg::Engine
{
	void PhysicsCollision::SwapObjects()
	{
		std::swap(_thisActor, _otherActor);
		std::swap(_thisVelocity, _otherVelocity);
	}
}