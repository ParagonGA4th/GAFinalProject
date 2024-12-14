#include "Stage1GUIHandler.h"
#include "TransformSimpleStorage.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgGraphics.h"
#include "PlayerCombatSector.h"
#include "TotalGameManager.h"
#include "GolemBossBehaviour.h"

#include "PlayerHandler.h"
#include "PauseBox.h"
#include "PauseOptionBox.h"
#include "CombatSystem.h"
#include "EventList_GameFlowRelated.h"
#include "EventList_PlayerRelated.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	Stage1GUIHandler::Stage1GUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	Stage1GUIHandler::~Stage1GUIHandler()
	{
		if (_staminaBillboardObject != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_staminaBillboardObject);
			delete _staminaBillboardObject;
		}
	}

	void Stage1GUIHandler::GrabManagedObjects()
	{
		//��� GUI Handler���� ȣ���ؾ� �ϴ� �Լ��̴�.
		GrabOrganizeAllGUI(_object);

		//��� Abstract�̴�. ������ GrabManaged���� ȣ��Ǿ����.
		AssignPointersToGUI();

		//�̴� Stamina ����� ���ֱ� ����. Object. _isActive�� ������ ������� ����س��´�.
		SetupStaminaBillboardRenderObject();

		//GolemBoss Object.
		SetupGolemBossHealthBar();

		_golemBossBehaviour = _object->GetScene()->FindSingleComponentInScene<GolemBossBehaviour>();
		assert(_golemBossBehaviour != nullptr);

		SetTutorialExplainUI(_object);
	}

	void Stage1GUIHandler::Start()
	{
		GetLifeUIObjects(_object);

		_staminaBillboardObject->SetActive(true);

		//_identifier�� �̺�Ʈ �߰��� ���� �þ ���̱⿡, ���� ���� Subscribe�ؾ� �� ���̴�.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_OnGolemBossGameAreaEnter::_identifier,
			std::bind(&Stage1GUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		// == GolemBoss ���� �Ŷ� ���� �ǹ�.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_EnableJump::_identifier,
			std::bind(&Stage1GUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		CombatSystem::GetInstance(nullptr)->Subscribe(Event_PlayerDeath::_identifier,
			std::bind(&Stage1GUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		_golemBossBar_Fill->SetActive(false);
		_golemBossBar_Frame->SetActive(false);
		_golemBossBar_Back->SetActive(false);
	}

	void Stage1GUIHandler::Update()
	{
		MatchUpdateStaminaToRO();
		MatchUpdateBossHealthBar();
		UpdateLife();
	}

	void Stage1GUIHandler::AssignPointersToGUI()
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

	void Stage1GUIHandler::AdditionalReset()
	{
		_pauseOptionBox->ResetAll();
	}

	void Stage1GUIHandler::SetupStaminaBillboardRenderObject()
	{
		_staminaBillboardObject = new Pg::Data::VisualEffectRenderObject();
		//EffectObject ���.
		_pgGraphics->RegisterEffectObject("Effect_StaminaStats", _staminaBillboardObject);
		_staminaBillboardObject->SetActive(false);
	}

	void Stage1GUIHandler::MatchUpdateStaminaToRO()
	{
		CombatSystem* tCombatSystem = CombatSystem::GetInstance(nullptr);

		//Offset �༭ ����.
		_staminaBillboardObject->_position = _playerTransform->_position + Pg::Math::PGFLOAT3(0, STAMINA_GUI_Y_OFFSET, 0);
		_staminaBillboardObject->_scale = { 1.0f, 0.3f, 1.0f };

		//Stamina �ޱ�.
		*_staminaTextureIndexPointer = tCombatSystem->GetPlayerStamina();

		//PG_WARN("STAMINA : {0}", *_staminaTextureIndexPointer);
	}

	void Stage1GUIHandler::CleanOnSceneChange()
	{
		_staminaBillboardObject->SetActive(false);
		_isGolemBossMet = false;
	}

	void Stage1GUIHandler::SetupGolemBossHealthBar()
	{
		Pg::Data::GameObject* tGolemBossBarObject = _object->GetScene()->FindObjectWithName("GolemBossHealthBar");
		assert(tGolemBossBarObject != nullptr);
		_golemBossBar_Fill = tGolemBossBarObject->GetComponent<Pg::Data::ImageRenderer>();
		assert(_golemBossBar_Fill != nullptr);

		Pg::Data::GameObject* tGolemBossFrameObject = _object->GetScene()->FindObjectWithName("GolemBossHealthBar_Frame");
		assert(tGolemBossFrameObject != nullptr);
		_golemBossBar_Frame = tGolemBossFrameObject->GetComponent<Pg::Data::ImageRenderer>();
		assert(_golemBossBar_Frame != nullptr);

		Pg::Data::GameObject* tGolemBossBackObject = _object->GetScene()->FindObjectWithName("GolemBossHealthBar_Back");
		assert(tGolemBossBackObject != nullptr);
		_golemBossBar_Back = tGolemBossBackObject->GetComponent<Pg::Data::ImageRenderer>();
		assert(_golemBossBar_Back != nullptr);

		_golemBossBar_Fill->SetActive(false);
		_golemBossBar_Frame->SetActive(false);
		_golemBossBar_Back->SetActive(false);
	}

	void Stage1GUIHandler::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		//���� Event_OnGolemBossGameAreaEnter -> Boss Bar Enabled.
		if (e.GetIdentifier() == Event_OnGolemBossGameAreaEnter::_identifier)
		{
			_isGolemBossMet = true;

			_golemBossBar_Fill->SetActive(true);
			_golemBossBar_Frame->SetActive(true);
			_golemBossBar_Back->SetActive(true);
		}
		else if (e.GetIdentifier() == Event_EnableJump::_identifier)
		{
			_golemBossBar_Fill->SetActive(false);
			_golemBossBar_Frame->SetActive(false);
			_golemBossBar_Back->SetActive(false);
		}
		else if (e.GetIdentifier() == Event_PlayerDeath::_identifier)
		{
			_golemBossBar_Fill->SetActive(_isGolemBossMet);
			_golemBossBar_Frame->SetActive(_isGolemBossMet);
			_golemBossBar_Back->SetActive(_isGolemBossMet);
		}
	}

	void Stage1GUIHandler::MatchUpdateBossHealthBar()
	{
		if (_golemBossBar_Fill->GetActive())
		{
			float tHP = std::clamp<float>(_golemBossBehaviour->ReturnBaseMonsterInfo()->GetMonsterHp(), 0.f, GolemBossBehaviour::MAX_GOLEM_BOSS_HEALTH);
			float tRatio = tHP / GolemBossBehaviour::MAX_GOLEM_BOSS_HEALTH;
			tRatio *= 100.f;
			_golemBossBar_Fill->SetFillRatio(tRatio);
		}
	}

}
