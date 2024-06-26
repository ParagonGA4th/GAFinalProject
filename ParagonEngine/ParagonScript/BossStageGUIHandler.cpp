#include "BossStageGUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

#include "PlayerHandler.h"
#include "PauseBox.h"

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
		//개별적으로 Object 이름과 값을 찾아서, 할당해준다.
		PlayerHandler* tPH = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		assert(tPH != nullptr);
		_managedGuiObjectList.at("HealthBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerHealthPointPointerConst(), nullptr, nullptr,
			tPH->MAX_PLAYER_HEALTH, NULL, NULL);

		//GUIHandler 나오기 전 작업물, 하드 셋.
		_pauseBox = _object->GetScene()->FindSingleComponentInScene<PauseBox>();
		assert(_pauseBox != nullptr);
	}

	void BossStageGUIHandler::AdditionalReset()
	{
		_pauseBox->ResetAll();
	}
	
	

}
