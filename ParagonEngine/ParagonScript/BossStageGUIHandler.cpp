#include "BossStageGUIHandler.h"
#include "TransformSimpleStorage.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonAPI/PgTime.h"
#include "PlayerCombatSector.h"
#include "TotalGameManager.h"
#include "BossBehaviour.h"

#include "PlayerHandler.h"
#include "PauseOptionBox.h"
#include "CombatSystem.h"
#include "EventList_GameFlowRelated.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	BossStageGUIHandler::BossStageGUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
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
		//��� GUI Handler���� ȣ���ؾ� �ϴ� �Լ��̴�.
		GrabOrganizeAllGUI(_object);

		//��� Abstract�̴�. ������ GrabManaged���� ȣ��Ǿ����.
		AssignPointersToGUI();

		//�̴� Stamina ����� ���ֱ� ����. Object. _isActive�� ������ ������� ����س��´�.
		SetupStaminaBillboardRenderObject();

		//Boss Bar ������Ʈ�� ��������.
		SetupBossHealthBar();

		SetTutorialExplainUI(_object);

		_finalBossBehaviour = _object->GetScene()->FindSingleComponentInScene<BossBehaviour>();
		assert(_finalBossBehaviour != nullptr);
		
		auto _boss = _object->GetScene()->FindObjectWithName("BossUI");
		_bossUI = _boss->GetComponent<Pg::Data::ImageRenderer>();
	}

	void BossStageGUIHandler::Start()
	{
		GetLifeUIObjects(_object);

		_staminaBillboardObject->SetActive(true);

		//_identifier�� �̺�Ʈ �߰��� ���� �þ ���̱⿡, ���� ���� Subscribe�ؾ� �� ���̴�.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_OnFinalBossGameAreaEnter::_identifier,
			std::bind(&BossStageGUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		// == GolemBoss ���� �Ŷ� ���� �ǹ�.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_OnFinalBossDeathGameWin::_identifier,
			std::bind(&BossStageGUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		_finalBossBar_Fill->SetActive(true);
		_finalBossBar_Frame->SetActive(true);
		_finalBossBar_Back->SetActive(true);
		_bossUI->SetActive(true);
	}

	void BossStageGUIHandler::Update()
	{
		MatchUpdateStaminaToRO();
		MatchUpdateBossHealthBar();
		UpdateLife();


		dTime = dTime + _pgTime->GetDeltaTime();

		if (dTime >= 3.f)
		{
			_bossUI->SetActive(false);
			dTime = 0.f;
		}
	}

	void BossStageGUIHandler::AssignPointersToGUI()
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

	void BossStageGUIHandler::AdditionalReset()
	{
		_pauseOptionBox->ResetAll();
	}

	void BossStageGUIHandler::SetupStaminaBillboardRenderObject()
	{
		_staminaBillboardObject = new Pg::Data::VisualEffectRenderObject();
		//EffectObject ���.
		_pgGraphics->RegisterEffectObject("Effect_StaminaStats", _staminaBillboardObject);
		_staminaBillboardObject->SetActive(false);
	}

	void BossStageGUIHandler::MatchUpdateStaminaToRO()
	{
		CombatSystem* tCombatSystem = CombatSystem::GetInstance(nullptr);

		//Offset �༭ ����.
		_staminaBillboardObject->_position = _playerTransform->_position + Pg::Math::PGFLOAT3(0, STAMINA_GUI_Y_OFFSET, 0);
		_staminaBillboardObject->_scale = { 1.0f, 0.3f, 1.0f };

		//Stamina �ޱ�.
		*_staminaTextureIndexPointer = tCombatSystem->GetPlayerStamina();

		//PG_WARN("STAMINA : {0}", *_staminaTextureIndexPointer);
	}

	void BossStageGUIHandler::CleanOnSceneChange()
	{
		_staminaBillboardObject->SetActive(false);
	}

	void BossStageGUIHandler::SetupBossHealthBar()
	{
		//Boss Health Bar.
		Pg::Data::GameObject* tBar = _object->GetScene()->FindObjectWithName("GolemBossHealthBar");
		assert(tBar != nullptr);
		_finalBossBar_Fill = tBar->GetComponent<Pg::Data::ImageRenderer>();
		assert(_finalBossBar_Fill != nullptr);

		Pg::Data::GameObject* tFrame = _object->GetScene()->FindObjectWithName("GolemBossHealthBar_Frame");
		assert(tFrame != nullptr);
		_finalBossBar_Frame = tFrame->GetComponent<Pg::Data::ImageRenderer>();
		assert(_finalBossBar_Frame != nullptr);

		Pg::Data::GameObject* tBack = _object->GetScene()->FindObjectWithName("GolemBossHealthBar_Back");
		assert(tBack != nullptr);
		_finalBossBar_Back = tBack->GetComponent<Pg::Data::ImageRenderer>();
		assert(_finalBossBar_Back != nullptr);

		_finalBossBar_Fill->SetActive(false);
		_finalBossBar_Frame->SetActive(false);
		_finalBossBar_Back->SetActive(false);
	}

	void BossStageGUIHandler::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		//���� Event_OnGolemBossGameAreaEnter -> Boss Bar Enabled.
		if (e.GetIdentifier() == Event_OnFinalBossGameAreaEnter::_identifier)
		{
			_finalBossBar_Fill->SetActive(true);
			_finalBossBar_Frame->SetActive(true);
			_finalBossBar_Back->SetActive(true);
			_bossUI->SetActive(true);
		}
		else if (e.GetIdentifier() == Event_OnFinalBossDeathGameWin::_identifier)
		{
			_finalBossBar_Fill->SetActive(false);
			_finalBossBar_Frame->SetActive(false);
			_finalBossBar_Back->SetActive(false);
		}
	}

	void BossStageGUIHandler::MatchUpdateBossHealthBar()
	{
		if (_finalBossBar_Fill->GetActive())
		{
			float tHP = std::clamp<float>(_finalBossBehaviour->ReturnBaseMonsterInfo()->GetMonsterHp(), 0.f, BossBehaviour::MAX_BOSS_HEALTH);
			float tRatio = tHP / BossBehaviour::MAX_BOSS_HEALTH;
			tRatio *= 100.f;
			_finalBossBar_Fill->SetFillRatio(tRatio);
		}
	}

}
