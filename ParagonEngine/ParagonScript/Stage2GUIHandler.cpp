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


		{
			_managedGuiObjectList.at("Skill1_WhiteFill")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_isStartedFireSkillChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isFireAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkFireAttack),
				PlayerCombatSector::FIRE_ATTACK_COOLDOWN_TIME, PlayerCombatSector::FIRE_ATTACK_REQUIRED_MANA, NULL);
		}

		{
			_managedGuiObjectList.at("Skill2_WhiteFill")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_isStartedIceSkillChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isIceAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkIceAttack),
				PlayerCombatSector::ICE_ATTACK_COOLDOWN_TIME, PlayerCombatSector::ICE_ATTACK_REQUIRED_MANA, NULL);
		}

		{
			_managedGuiObjectList.at("UltimateUI_WhiteFill")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_isStartedUltimateAttackChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isUltimateAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkUltimateAttack),
				PlayerCombatSector::ULTIMATE_ATTACK_COOLDOWN_TIME, PlayerCombatSector::ULTIMATE_ATTACK_REQUIRED_MANA, NULL);
		}

		//GUIHandler 나오기 전 작업물, 하드 셋.
		_pauseBox = _object->GetScene()->FindSingleComponentInScene<PauseBox>();
		assert(_pauseBox != nullptr);
	}

	void Stage2GUIHandler::AdditionalReset()
	{
		_pauseBox->ResetAll();
	}

}
