#include "BossStageAreaHandler.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"

#include "../ParagonUtil/CustomAssert.h"

namespace Pg::DataScript
{
	BossStageAreaHandler::BossStageAreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseAreaHandler(ENTIRE_AREA_COUNT)
	{

	}

	void BossStageAreaHandler::GrabManagedObjects()
	{
		//모든 DerivedClass들은 GrabManagedObjects에서 해당 함수 호출해야 된다.
		GrabOrganizeAllAreas(_object);
	}

	void BossStageAreaHandler::Start()
	{

	}

	void BossStageAreaHandler::Update()
	{

	}
}