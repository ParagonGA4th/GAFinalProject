#pragma once
#include <PxPhysicsAPI.h>
namespace Pg::Engine
{
	class PhysicsCallback : public physx::PxSimulationEventCallback
	{
	public:
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {
			// Handle contact events (collision start or ongoing)
			for (physx::PxU32 i = 0; i < nbPairs; ++i) {
				const physx::PxContactPair& cp = pairs[i];

				if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) 
				{

					// Collision started
					// You can access pairHeader and cp to get information about the collision
					// pairHeader.actors[0] and pairHeader.actors[1] are the colliding actors (rigid bodies)
					//if (pairHeader.actors[0]->)
				}

				// Add more conditions as needed to check for other collision events
			}
		}

		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override {
			// Handle trigger events (overlap, not physical collision)
		}
	};
}

