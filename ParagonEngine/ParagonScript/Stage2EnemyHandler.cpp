#include "Stage2EnemyHandler.h"
#include "Stage2AreaHandler.h"

namespace Pg::DataScript
{
	Stage2EnemyHandler::Stage2EnemyHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseEnemyHandler(Stage2AreaHandler::ENTIRE_AREA_COUNT)
	{

	}

	void Stage2EnemyHandler::GrabManagedObjects()
	{
		//모든 자식 EnemyHandler들은 부모의 함수를 호출해야 한다.
		GrabOrganizeAllEnemies(_object);
	}

	void Stage2EnemyHandler::Start()
	{

	}

	void Stage2EnemyHandler::Update()
	{

	}

}