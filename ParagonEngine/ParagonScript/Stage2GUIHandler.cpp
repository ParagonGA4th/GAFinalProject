#include "Stage2GUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

#include "PlayerHandler.h"
#include "PlayerCombatSector.h"
#include "PauseBox.h"

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
		//개별적으로 Object 이름과 값을 찾아서, 할당해준다.
		PlayerHandler* tPH = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		assert(tPH != nullptr);
		_managedGuiObjectList.at("HealthBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerHealthPointPointerConst(), nullptr, nullptr,
			tPH->MAX_PLAYER_HEALTH, NULL, NULL);

		_managedGuiObjectList.at("ManaBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerManaPointPointerConst(), nullptr, nullptr,
			tPH->MAX_PLAYER_MANA, NULL, NULL);

		_managedGuiObjectList.at("Skill1_WhiteFill")._guiComponent->ReceiveDependentPointers(
			(void*)&(tPH->GetPlayerCombatSector()->_isStartedFireSkillChargeTime), nullptr, nullptr,
			PlayerCombatSector::FIRE_ATTACK_COOLDOWN_TIME, NULL, NULL);

		_managedGuiObjectList.at("Skill2_WhiteFill")._guiComponent->ReceiveDependentPointers(
			(void*)&(tPH->GetPlayerCombatSector()->_isStartedIceSkillChargeTime), nullptr, nullptr,
			PlayerCombatSector::ICE_ATTACK_COOLDOWN_TIME, NULL, NULL);

		_managedGuiObjectList.at("UltimateUI_WhiteFill")._guiComponent->ReceiveDependentPointers(
			(void*)&(tPH->GetPlayerCombatSector()->_isStartedUltimateAttackChargeTime), nullptr, nullptr,
			PlayerCombatSector::ULTIMATE_ATTACK_COOLDOWN_TIME, NULL, NULL);

		//GUIHandler 나오기 전 작업물, 하드 셋.
		_pauseBox = _object->GetScene()->FindSingleComponentInScene<PauseBox>();
		assert(_pauseBox != nullptr);
	}

	void Stage2GUIHandler::AdditionalReset()
	{
		_pauseBox->ResetAll();
	}

}
