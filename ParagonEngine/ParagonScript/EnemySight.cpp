#include "EnemySight.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/LayerMask.h"


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

		if (c->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
		{
			_playerDetected = true;
		}
	}

	void EnemySight::OnTriggerExit(Pg::Data::Collider* c)
	{

	}

}