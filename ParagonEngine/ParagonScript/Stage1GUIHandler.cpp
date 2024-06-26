#include "Stage1GUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

#include "PlayerHandler.h"

namespace Pg::DataScript
{
	Stage1GUIHandler::Stage1GUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void Stage1GUIHandler::GrabManagedObjects()
	{
		//모든 GUI Handler들이 호출해야 하는 함수이다.
		GrabOrganizeAllGUI(_object);
		
		//얘는 Abstract이다. 무조건 GrabManaged에서 호출되었어야.
		AssignPointersToGUI();
	}

	void Stage1GUIHandler::Start()
	{

	}

	void Stage1GUIHandler::Update()
	{

	}

	void Stage1GUIHandler::AssignPointersToGUI()
	{
		//개별적으로 Object 이름과 값을 찾아서, 할당해준다.
		PlayerHandler* tPH = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		assert(tPH != nullptr);
		_managedGuiObjectList.at("HealthBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerHealthPointPointerConst(), nullptr, nullptr, 
			tPH->MAX_PLAYER_HEALTH, NULL, NULL);
	}

}
