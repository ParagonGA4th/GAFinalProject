#include "BossStageGUIHandler.h"
#include "TransformSimpleStorage.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgGraphics.h"
#include "PlayerCombatSector.h"
#include "TotalGameManager.h"

#include "PlayerHandler.h"
#include "PauseBox.h"
#include "CombatSystem.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	BossStageGUIHandler::BossStageGUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	BossStageGUIHandler::~BossStageGUIHandler()
	{
		if (_staminaBillboardObject != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_staminaBillboardObject);
			delete _staminaBillboardObject;
		}
	}

	void BossStageGUIHandler::GrabManagedObjects()
	{
		//모든 GUI Handler들이 호출해야 하는 함수이다.
		GrabOrganizeAllGUI(_object);

		//얘는 Abstract이다. 무조건 GrabManaged에서 호출되었어야.
		AssignPointersToGUI();

		//이는 Stamina 등록을 해주기 위해. Object. _isActive만 꺼놓는 방식으로 등록해놓는다.
		SetupStaminaBillboardRenderObject();
	}

	void BossStageGUIHandler::Start()
	{
		_staminaBillboardObject->SetActive(true);
	}

	void BossStageGUIHandler::Update()
	{
		MatchUpdateStaminaToRO();
	}

	void BossStageGUIHandler::AssignPointersToGUI()
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

		//플레이어 가져오기.
		_playerTransform = &(tPH->_object->_transform);

		_staminaTextureIndexPointer = _pgGraphics->GetEffectTextureIndexPointer("Effect_StaminaStats");
		assert(_staminaTextureIndexPointer != nullptr);
	}

	void BossStageGUIHandler::AdditionalReset()
	{
		_pauseBox->ResetAll();
	}

	void BossStageGUIHandler::SetupStaminaBillboardRenderObject()
	{
		_staminaBillboardObject = new Pg::Data::VisualEffectRenderObject();
		//EffectObject 등록.
		_pgGraphics->RegisterEffectObject("Effect_StaminaStats", _staminaBillboardObject);
		_staminaBillboardObject->SetActive(false);
	}

	void BossStageGUIHandler::MatchUpdateStaminaToRO()
	{
		CombatSystem* tCombatSystem = CombatSystem::GetInstance(nullptr);

		//Offset 줘서 띄우기.
		_staminaBillboardObject->_position = _playerTransform->_position + Pg::Math::PGFLOAT3(0, STAMINA_GUI_Y_OFFSET, 0);
		_staminaBillboardObject->_scale = { 1.0f, 0.3f, 1.0f };

		//Stamina 받기.
		*_staminaTextureIndexPointer = tCombatSystem->GetPlayerStamina();

		//PG_WARN("STAMINA : {0}", *_staminaTextureIndexPointer);
	}

	void BossStageGUIHandler::CleanOnSceneChange()
	{
		_staminaBillboardObject->SetActive(false);
	}

}
