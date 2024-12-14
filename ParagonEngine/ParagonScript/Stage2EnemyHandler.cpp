#include "Stage2EnemyHandler.h"
#include "Stage2AreaHandler.h"

namespace Pg::DataScript
{
	Stage2EnemyHandler::Stage2EnemyHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseEnemyHandler(Stage2AreaHandler::ENTIRE_AREA_COUNT)
	{

	}

	void Stage2EnemyHandler::GrabManagedObjects()
	{
		//��� �ڽ� EnemyHandler���� �θ��� �Լ��� ȣ���ؾ� �Ѵ�.
		GrabOrganizeAllEnemies(_object);
	}

	void Stage2EnemyHandler::Start()
	{

	}

	void Stage2EnemyHandler::Update()
	{

	}

}