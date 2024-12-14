#include "Stage2GUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

#include "PlayerHandler.h"
#include "PlayerCombatSector.h"
#include "PauseOptionBox.h"

#include "../ParagonAPI/PgGraphics.h"
#include "CombatSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	Stage2GUIHandler::Stage2GUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	Stage2GUIHandler::~Stage2GUIHandler()
	{
		if (_staminaBillboardObject != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_staminaBillboardObject);
			delete _staminaBillboardObject;
		}
	}

	void Stage2GUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();

		//�̴� Stamina ����� ���ֱ� ����. Object. _isActive�� ������ ������� ����س��´�.
		SetupStaminaBillboardRenderObject();

		SetTutorialExplainUI(_object);
	}

	void Stage2GUIHandler::Start()
	{
		GetLifeUIObjects(_object);

		_staminaBillboardObject->SetActive(true);
	}

	void Stage2GUIHandler::Update()
	{
		MatchUpdateStaminaToRO();
		UpdateLife();
	}

	void Stage2GUIHandler::AssignPointersToGUI()
	{
		//���������� Object �̸��� ���� ã�Ƽ�, �Ҵ����ش�.
		PlayerHandler* tPH = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		assert(tPH != nullptr);
		_managedGuiObjectList.at("HealthBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerHealthPointPointerConst(), nullptr, nullptr,
			tPH->MAX_PLAYER_HEALTH, NULL, NULL);

		_managedGuiObjectList.at("ManaBar")._guiComponent->ReceiveDependentPointers(
			(void*)tPH->ReturnPlayerManaPointPointerConst(), nullptr, nullptr,
			tPH->MAX_PLAYER_MANA, NULL, NULL);


		{
			_managedGuiObjectList.at("Skill2_WhiteFill")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_isStartedFireSkillChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isFireAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkFireAttack),
				PlayerCombatSector::FIRE_ATTACK_COOLDOWN_TIME, PlayerCombatSector::FIRE_ATTACK_REQUIRED_MANA, NULL);
		}

		{
			_managedGuiObjectList.at("Skill1_WhiteFill")._guiComponent->ReceiveDependentPointers(
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

		//��ٿ� �Ҵ�. Near Mouse Cursor.
		{
			_managedGuiObjectList.at("GUI_StrongAttackCooldown_Frame")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_startedStrongAttackChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isStrongAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkStrongAttack),
				PlayerCombatSector::STRONG_ATTACK_COOLDOWN_TIME, NULL, NULL);
		}

		//GUIHandler ������ �� �۾���, �ϵ� ��.
		_pauseOptionBox = _object->GetScene()->FindSingleComponentInScene<PauseOptionBox>();
		assert(_pauseOptionBox != nullptr);

		//�÷��̾� ��������.
		_playerTransform = &(tPH->_object->_transform);

		_staminaTextureIndexPointer = _pgGraphics->GetEffectTextureIndexPointer("Effect_StaminaStats");
		assert(_staminaTextureIndexPointer != nullptr);
	}

	void Stage2GUIHandler::AdditionalReset()
	{
		//_pauseBox->ResetAll();
		_pauseOptionBox->ResetAll();
	}

	void Stage2GUIHandler::SetupStaminaBillboardRenderObject()
	{
		_staminaBillboardObject = new Pg::Data::VisualEffectRenderObject();
		//EffectObject ���.
		_pgGraphics->RegisterEffectObject("Effect_StaminaStats", _staminaBillboardObject);
		_staminaBillboardObject->SetActive(false);
	}

	void Stage2GUIHandler::MatchUpdateStaminaToRO()
	{
		CombatSystem* tCombatSystem = CombatSystem::GetInstance(nullptr);

		//Offset �༭ ����.
		_staminaBillboardObject->_position = _playerTransform->_position + Pg::Math::PGFLOAT3(0, STAMINA_GUI_Y_OFFSET, 0);
		_staminaBillboardObject->_scale = { 1.0f, 0.3f, 1.0f };

		//Stamina �ޱ�.
		*_staminaTextureIndexPointer = tCombatSystem->GetPlayerStamina();

		//PG_WARN("STAMINA : {0}", *_staminaTextureIndexPointer);
	}

	void Stage2GUIHandler::CleanOnSceneChange()
	{
		_staminaBillboardObject->SetActive(false);
	}



}
