#include "AISeight.h"

#include "../ParagonData/DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

AISeight::AISeight(Pg::Data::GameObject* obj) :
	Component(obj)
{

}

void AISeight::Start()
{

}

void AISeight::Update()
{

}

void AISeight::OnTriggerEnter(Pg::Data::Collider* c)
{
	//적 시야 감지될 시.
	if (c->GetLayer() == 0)
	{
		PG_TRACE("적 감지!!");
	}
}
