#include "Stage2GUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

namespace Pg::DataScript
{
	Stage2GUIHandler::Stage2GUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void Stage2GUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();
	}

	void Stage2GUIHandler::Start()
	{

	}

	void Stage2GUIHandler::Update()
	{

	}

	void Stage2GUIHandler::AssignPointersToGUI()
	{

	}
}
