#include "Stage2AreaHandler.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"

#include "../ParagonUtil/CustomAssert.h"

#include <algorithm>

namespace Pg::DataScript
{
	Stage2AreaHandler::Stage2AreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseAreaHandler(ENTIRE_AREA_COUNT)
	{

	}

	void Stage2AreaHandler::GrabManagedObjects()
	{
		//모든 DerivedClass들은 GrabManagedObjects에서 해당 함수 호출해야 된다.
		GrabOrganizeAllAreas(_object);
	}

	void Stage2AreaHandler::Start()
	{

	}

	void Stage2AreaHandler::Update()
	{

	}

}