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
		//��� DerivedClass���� GrabManagedObjects���� �ش� �Լ� ȣ���ؾ� �ȴ�.
		GrabOrganizeAllAreas(_object);
	}

	void BossStageAreaHandler::Start()
	{

	}

	void BossStageAreaHandler::Update()
	{

	}
}