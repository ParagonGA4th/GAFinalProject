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
	//if (_afterPlayerExit >= 0.0f)
	//{
	//	_afterPlayerExit += tTime->GetDeltaTime();

	//	if (_afterPlayerExit >= 3.0f)
	//	{
	//		col->SetActive(true);
	//		_afterPlayerExit = 0.0f;
	//		PG_TRACE("플레이어 감지 해제.");
	//	}
	//}

	//PG_TRACE(_afterPlayerExit);
}

void AISeight::OnTriggerEnter(Pg::Data::Collider* c)
{
	PG_TRACE("충돌함!!");

	if (c->GetLayer() == 0)
	{
		_playerDetected = true;
	}
}

void AISeight::OnTriggerExit(Pg::Data::Collider* c)
{
	//_playerDetected = false;
	//if (c->GetLayer() == 0)
	//{
	//	if (_afterPlayerExit >= 0.0f)
	//	{
	//		_afterPlayerExit += tTime->GetDeltaTime();

	//		if (_afterPlayerExit >= 2.0f)
	//		{
	//			col->SetActive(true);
	//			_afterPlayerExit = 0.0f;
	//		}
	//	}
	//	//_afterPlayerExit = tTime->GetDeltaTime();
	//	//col->SetActive(true);
}

