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
	// Time
	auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
	tTime = &tTimeSystem;

	// Physic
	auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
	tPhysic = &tPhysicSystem;

}

void AISeight::BeforePhysicsAwake()
{
	col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
}

void AISeight::Start()
{

}

void AISeight::Update()
{

}

void AISeight::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
{

}

void AISeight::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
{

}

