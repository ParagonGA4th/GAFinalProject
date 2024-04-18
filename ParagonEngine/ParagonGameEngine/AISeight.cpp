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
	_playerDetected = true;
	//Pg::Math::PGFLOAT3 outHitPoint;

	////적 시야 감지될 시.
	//if (c->GetLayer() == 0)
	//{
	//	col->SetActive(false);
	//	tPhysic->MakeRayCast({ _object->_transform._position.x + 0.8f,
	//					_object->_transform._position.y - 0.5f,
	//					_object->_transform._position.z + 0.8f }, { 0.0f,0.0f,1.0f }, 10.0f, outHitPoint, 0);
	//	PG_TRACE("플레이어 감지!!");
	//}

	/*if (_afterPlayerExit >= 0.0f)
	{
		_afterPlayerExit += tTime->GetDeltaTime();

		if (_afterPlayerExit >= 2.0f)
		{
			col->SetActive(true);
			_afterPlayerExit = 0.0f;
		}
	}*/
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

