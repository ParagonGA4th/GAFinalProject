#include "BossStageGUIHandler.h"
#include "TransformSimpleStorage.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgGraphics.h"
#include "PlayerCombatSector.h"
#include "TotalGameManager.h"
#include "BossBehaviour.h"

#include "PlayerHandler.h"
#include "PauseBox.h"
#include "CombatSystem.h"
#include "EventList_GameFlowRelated.h"

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

		//Boss Bar 오브젝트들 가져오기.
		SetupBossHealthBar();

		_finalBossBehaviour = _object->GetScene()->FindSingleComponentInScene<BossBehaviour>();
		assert(_finalBossBehaviour != nullptr);
	}

	void BossStageGUIHandler::Start()
	{
		_staminaBillboardObject->SetActive(true);

		//_identifier가 이벤트 추가에 따라 늘어날 것이기에, 여러 개를 Subscribe해야 할 것이다.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_OnFinalBossGameAreaEnter::_identifier,
			std::bind(&BossStageGUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		// == GolemBoss 죽은 거랑 같은 의미.
		CombatSystem::GetInstance(nullptr)->Subscribe(Event_OnFinalBossDeathGameWin::_identifier,
			std::bind(&BossStageGUIHandler::HandleEvents, this, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		_finalBossBar_Fill->SetActive(true);
		_finalBossBar_Frame->SetActive(true);
		_finalBossBar_Back->SetActive(true);
	}

	void BossStageGUIHandler::Update()
	{
		MatchUpdateStaminaToRO();
		MatchUpdateBossHealthBar();
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

		//쿨다운 할당. Near Mouse Cursor.
		{
			_managedGuiObjectList.at("GUI_StrongAttackCooldown_Frame")._guiComponent->ReceiveDependentPointers(
				(void*)&(tPH->GetPlayerCombatSector()->_startedStrongAttackChargeTime),
				(void*)&(tPH->GetPlayerCombatSector()->_isStrongAttackStartEligible),
				(void*)&(tPH->GetPlayerCombatSector()->_checkStrongAttack),
				PlayerCombatSector::STRONG_ATTACK_COOLDOWN_TIME, NULL, NULL);
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
		//만약 Event_OnGolemBossGameAreaEnter -> Boss Bar Enabled.
		if (e.GetIdentifier() == Event_OnFinalBossGameAreaEnter::_identifier)
		{
			_finalBossBar_Fill->SetActive(true);
			_finalBossBar_Frame->SetActive(true);
			_finalBossBar_Back->SetActive(true);
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
