#include "Stage1AreaHandler.h"



namespace Pg::DataScript
{
	Stage1AreaHandler::Stage1AreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseAreaHandler(ENTIRE_AREA_COUNT)
	{

	}

	void Stage1AreaHandler::GrabManagedObjects()
	{
		//모든 DerivedClass들은 GrabManagedObjects에서 해당 함수 호출해야 된다.
		GrabOrganizeAllAreas(_object);
	}

	void Stage1AreaHandler::Start()
	{

	}

	void Stage1AreaHandler::Update()
	{

	}

	

}