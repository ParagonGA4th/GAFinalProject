#include "Enemy.h"

#include "../ParagonGameEngine/PhysicSystem.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

Enemy::Enemy(Pg::Data::GameObject* obj) :
	Component(obj)
{

}

void Enemy::Start()
{
	// Physic
	auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
	tPhysic = &tPhysicSystem;

	for (auto& iter : _object->_transform.GetChildren())
	{
		Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
		AISeight* aiSight = iter->_object->GetComponent<AISeight>();

		if (aiSight != nullptr)
		{
			colVec.push_back(staticCol);
			aiSightVec.push_back(aiSight);
		}
	}
}

void Enemy::Update()
{
	for (auto& it : aiSightVec)
	{
		if (it->_playerDetected == true)
		{
			//_colVecActive = false;

		}
		else
		{
			//_colVecActive = true;
		}
	}

	if (_colVecActive == false)
	{
		for (auto& iter : colVec)
		{
			//iter->SetActive(false);
		}

		//_colVecActive = true;
	}
	else
	{
		for (auto& iter : colVec)
		{
			//iter->SetActive(true);
		}
	}
}

void Enemy::OnTriggerEnter(Pg::Data::Collider* c)
{

}

void Enemy::OnTriggerExit(Pg::Data::Collider* c)
{

}
