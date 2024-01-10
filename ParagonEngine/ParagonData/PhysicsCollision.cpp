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
}