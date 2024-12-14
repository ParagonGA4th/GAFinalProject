#include "Stage1EnemyHandler.h"
#include "Stage1AreaHandler.h"
#include "../ParagonData/Scene.h"

#include <algorithm>

namespace Pg::DataScript
{
	Stage1EnemyHandler::Stage1EnemyHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseEnemyHandler(Stage1AreaHandler::ENTIRE_AREA_COUNT)
	{

	}

	void Stage1EnemyHandler::GrabManagedObjects()
	{
		//��� �ڽ� EnemyHandler���� �θ��� �Լ��� ȣ���ؾ� �Ѵ�.
		GrabOrganizeAllEnemies(_object);
	}

	void Stage1EnemyHandler::Start()
	{

	}

	void Stage1EnemyHandler::Update()
	{

	}


	

}