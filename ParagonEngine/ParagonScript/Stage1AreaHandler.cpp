#include "Stage1AreaHandler.h"



namespace Pg::DataScript
{
	Stage1AreaHandler::Stage1AreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj), BaseAreaHandler(ENTIRE_AREA_COUNT)
	{

	}

	void Stage1AreaHandler::GrabManagedObjects()
	{
		//��� DerivedClass���� GrabManagedObjects���� �ش� �Լ� ȣ���ؾ� �ȴ�.
		GrabOrganizeAllAreas(_object);
	}

	void Stage1AreaHandler::Start()
	{

	}

	void Stage1AreaHandler::Update()
	{

	}

	

}