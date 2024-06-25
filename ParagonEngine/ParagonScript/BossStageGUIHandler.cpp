#include "BossStageGUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

namespace Pg::DataScript
{
	BossStageGUIHandler::BossStageGUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void BossStageGUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();
	}

	void BossStageGUIHandler::Start()
	{

	}

	void BossStageGUIHandler::Update()
	{

	}

	void BossStageGUIHandler::AssignPointersToGUI()
	{

	}
	
	

}
