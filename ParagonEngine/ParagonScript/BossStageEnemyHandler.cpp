#include "BossStageEnemyHandler.h"
#include "BossStageAreaHandler.h"
#include "../ParagonData/Scene.h"

namespace Pg::DataScript
{
	BossStageEnemyHandler::BossStageEnemyHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseEnemyHandler(BossStageAreaHandler::ENTIRE_AREA_COUNT)
	{

	}

	void BossStageEnemyHandler::GrabManagedObjects()
	{
		GrabOrganizeAllEnemies(_object);
	}

	void BossStageEnemyHandler::Start()
	{

	}

	void BossStageEnemyHandler::Update()
	{

	}

}