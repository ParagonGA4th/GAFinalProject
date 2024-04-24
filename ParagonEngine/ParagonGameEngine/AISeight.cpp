#include "AISeight.h"

#include "../ParagonGameEngine/PhysicSystem.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

AISeight::AISeight(Pg::Data::GameObject* obj) :
	Component(obj)
{

}

void AISeight::Start()
{
	// Time
	auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
	tTime = &tTimeSystem;

	// Physic
	auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
	tPhysic = &tPhysicSystem;

	col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
}

void AISeight::Update()
{

}

void AISeight::OnTriggerEnter(Pg::Data::Collider* c)
{
	PG_TRACE("Ãæµ¹ÇÔ!!");

	if (c->GetLayer() == 0)
	{
		_playerDetected = true;
	}
}

void AISeight::OnTriggerExit(Pg::Data::Collider* c)
{

}

