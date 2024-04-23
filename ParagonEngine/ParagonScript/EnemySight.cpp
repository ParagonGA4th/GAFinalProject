#include "EnemySight.h"
#include "../ParagonData/Collider.h"


namespace Pg::DataScript
{

	EnemySight::EnemySight(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void EnemySight::Start()
	{

	}

	void EnemySight::Update()
	{

	}

	void EnemySight::OnTriggerEnter(Pg::Data::Collider* c)
	{
		//PG_TRACE("Ãæµ¹ÇÔ!!");

		if (c->GetLayer() == 0)
		{
			_playerDetected = true;
		}
	}

	void EnemySight::OnTriggerExit(Pg::Data::Collider* c)
	{

	}

}