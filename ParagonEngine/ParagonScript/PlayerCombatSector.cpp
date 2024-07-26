#include "PlayerCombatSector.h"
#include "ArrowLogic.h"
#include "UltimateArrowLogic.h"
#include "StrongAttackLogic.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"
#include "BossBehaviour.h"
#include "CombatSystem.h"
#include "ComboSystem.h"
#include "EventList_PlayerRelated.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"

#include <singleton-cpp/singleton.h>


namespace Pg::DataScript
{
	PlayerCombatSector::PlayerCombatSector(PlayerHandler* playerHandler) : _playerHandler(playerHandler), _object(playerHandler->_object)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
	}

	void PlayerCombatSector::GrabManagedObjects()
	{
		FindAllArrowsInMap();

		_strongAttackLogic = _object->GetScene()->FindSingleComponentInScene<StrongAttackLogic>();
		assert(_strongAttackLogic != nullptr);


	}

	void PlayerCombatSector::BeforePhysicsAwake()
	{
		Pg::Data::GameObject* tUltimateArrow;

		//궁극기 관련 요소 모두 찾기.
		tUltimateArrow = _object->GetScene()->FindObjectWithName("UltimateArrow");
		assert(tUltimateArrow != nullptr);
		_ulArrowLogic = tUltimateArrow->GetComponent<UltimateArrowLogic>();
		assert(_ulArrowLogic != nullptr);

		if (_playerHandler->_object->GetScene()->GetSceneName() == "BossStage")
		{
			auto _boss = _object->GetScene()->FindObjectWithName("Boss");
			assert(_boss != nullptr);
			_bossBehaviour = _boss->GetComponent<BossBehaviour>();
			assert(_bossBehaviour != nullptr);
		}
	}

	void PlayerCombatSector::Awake()
	{

	}

	void PlayerCombatSector::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void PlayerCombatSector::Update()
	{
		//지상이 형 로직은 이미 합쳐졌다.
		if (_bossBehaviour->GetProhibitAttack() == false)
		{
			ProcessInputsForActiveSkills();
			ProcessInputsForUltimateAttack();
			ProcessInputsForStrongAttack();
			UpdateForGUIVariables();
			AllAttacksLogic();
			//나머지 로직은 Combat System으로 이동.

			if (_isWaiting)
			{
				_attackWatingTime -= _pgTime->GetDeltaTime();
				if (_attackWatingTime <= std::numeric_limits<float>::epsilon())
				{
					_playerHandler->_meshRenderer->SetAnimation("PA_00001.pganim", true);
					_attackWatingTime = AFTER_ATTACK_WATING_TIME;
					_isWaiting = false;
				}
			}
		}
		//PG_WARN("ICE : {0}", _isStartedIceSkillChargeTime);
	}

	void PlayerCombatSector::FixedUpdate()
	{

	}

	void PlayerCombatSector::LateUpdate()
	{
	}

	void PlayerCombatSector::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void PlayerCombatSector::ResetAll()
	{
		// 공격 관련 변수들 리셋.
		_startedClickingTime = 0.f;
		_isStrongAttackStartEligible = true;
		_startedStrongAttackChargeTime = 0.f;

		_isUltimateAttackStartEligible = true;
		_isStartedUltimateAttackChargeTime = 0.f;

		_isIceAttackStartEligible = true;
		_isFireAttackStartEligible = true;

		_isStartedIceSkillChargeTime = 0.f;
		_isStartedFireSkillChargeTime = 0.f;

		//현재 공격 상태 리셋.
		_isStrongAttackingNow = false;
		_isUltimateAttackingNow = false;
		_isIceAttackingNow = false;
		_isFireAttackingNow = false;

		//단발성 아닌 공격들 위해 세팅.
		_isJustStrongAttackInvoked = false;
		_isJustUltimateAttackInvoked = false;

		_startedStrongAttackingTime = 0.f;

		//UI Manager : 내부 액티브스킬 GUI 초기 세팅 따로 해야 한다.
	}

	void PlayerCombatSector::ProcessInputsForActiveSkills()
	{
		// 일반 공격 : 그냥 지금처럼 화살 
		// 액티브면 불 / 얼음 화살 이렇게 스위칭이고,
		// Q - E로 바꾸면 즉시 화살 쏘는 것처럼 투사체 나감.
		//Q와 E를 누르면 Switch.

		if (_isFireAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyQ))
			{
				if (CheckActivateFireAttack())
				{
					//Event HandleEvents에서 나중에 구분해야 한다.
					_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), false, NULL);
					//이제 Cooldown 세자.
					_isFireAttackStartEligible = false;
					_isStartedFireSkillChargeTime = 0.f;

					_playerHandler->_meshRenderer->SetAnimation("PA_00009.pganim", false);
				}
			}
		}
		else
		{
			//쿨다운 세기.
			_isStartedFireSkillChargeTime += _pgTime->GetDeltaTime();

			if (_isStartedFireSkillChargeTime >= FIRE_ATTACK_COOLDOWN_TIME)
			{
				_isFireAttackStartEligible = true;
				_isStartedFireSkillChargeTime = 0.f;
			}
		}


		if (_isIceAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyE))
			{
				if (CheckActivateIceAttack())
				{
					//Event HandleEvents에서 나중에 구분해야 한다.
					_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), true, NULL);
					//이제 Cooldown 세자.
					_isIceAttackStartEligible = false;
					_isStartedIceSkillChargeTime = 0.f;

					_playerHandler->_meshRenderer->SetAnimation("PA_00009.pganim", false);
				}
			}
		}
		else
		{
			//쿨다운 세기.
			_isStartedIceSkillChargeTime += _pgTime->GetDeltaTime();

			if (_isStartedIceSkillChargeTime >= ICE_ATTACK_COOLDOWN_TIME)
			{
				_isIceAttackStartEligible = true;
				_isStartedIceSkillChargeTime = 0.f;
			}
		}
	}

	void PlayerCombatSector::ProcessInputsForStrongAttack()
	{
		//마우스 클릭한 순간부터 기록 시작.

		//만약 강공격을 쓸 수 있는 상태라면
		if (_isStrongAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft) ||
				_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MouseLeft) ||
				_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseRight) ||
				_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MouseRight) ||
				_playerHandler->GetPlayerMovementSector()->GetIsMoving())
			{
				_startedClickingTime = 0.f;
			}

			if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MouseRight))
			{
				_startedClickingTime += _pgTime->GetDeltaTime();

				//만약 2초보다 더 길게 클릭한다면 -> 강공격 발동.
				if (_startedClickingTime >= 1.0f)
				{
					//PG_ERROR("Counting : {0}", _startedClickingTime);
					if (CheckActivateStrongAttack())
					{
						_startedClickingTime = 0.f;
						_startedStrongAttackChargeTime = 0.f;
						_isStrongAttackStartEligible = false;
					}
				}
			}
		}
		else
		{
			//PG_ERROR("Cooldown : {0}", _startedStrongAttackChargeTime);
			// 강공격이 이제 불가능하다면, 다시 쿨다운 재충전을 위한 순간들이 필요하다.
			_startedStrongAttackChargeTime += _pgTime->GetDeltaTime();
			//3초 쿨다운 필요.
			if (_startedStrongAttackChargeTime >= STRONG_ATTACK_COOLDOWN_TIME)
			{
				//PG_ERROR("COOLDOWNDONE");
				//다시 호출될 수 있게.
				_isStrongAttackStartEligible = true;
				_startedStrongAttackChargeTime = 0.f;
			}
		}
	}

	void PlayerCombatSector::ProcessInputsForUltimateAttack()
	{
		if (_isUltimateAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyR))
			{
				//궁극기 성공하면
				if (CheckActivateUltimateAttack())
				{
					// 0으로 다시 돌려놓기. 쿨다운을 위해.
					_isStartedUltimateAttackChargeTime = 0.f;
					_isUltimateAttackStartEligible = false;
				}
			}
		}
		else
		{
			//아니면, 궁극기을 위해 쿨다운 재충전을 해야 한다.
			_isStartedUltimateAttackChargeTime += _pgTime->GetDeltaTime();
			if (_isStartedUltimateAttackChargeTime >= ULTIMATE_ATTACK_COOLDOWN_TIME)
			{
				_isUltimateAttackStartEligible = true;
				_isStartedUltimateAttackChargeTime = 0.f;
			}
		}
	}

	void PlayerCombatSector::FindAllArrowsInMap()
	{
		//ArrowLogic의 Awake에서 자신의 Tag를 이미 "TAG_Arrow"로 설정해놨었을 것이다.
		//모든 Arrow들 받아오기. 한 30개는 되어야 빈틈을 눈치를 못 챌 것이다.
		std::vector<Pg::Data::GameObject*> allObjects = _object->GetScene()->FindObjectsWithTag("TAG_Arrow");
		for (auto& it : allObjects)
		{
			auto tALogic = it->GetComponent<Pg::DataScript::ArrowLogic>();
			if (tALogic != nullptr)
			{
				tALogic->_playerBattleBehavior = _playerHandler;

				if (tALogic->_arrowType == -1)
				{
					//Ice.
					_iceArrowVec.push_back(tALogic);
				}
				else if (tALogic->_arrowType == 0)
				{
					//Normal.
					_normalArrowVec.push_back(tALogic);
				}
				else if (tALogic->_arrowType == 1)
				{
					//Fire.
					_fireArrowVec.push_back(tALogic);
				}
				else { assert(false && "서포트 안함 이 타입은."); }
			}
		}
		assert("");
		//if (_iceArrowVec.empty() ||

		//Material별로 다르게 셋해줘야 한다.
		//UltimateArrowAttack은 별도로 자식 객체가 될 것.
		//IceArrow : 15개 / FireArrow : 15개 / 일반 Arrow : 20개.
	}

	void PlayerCombatSector::AllAttacksLogic()
	{
		// 여기서 불 / 얼음을 나눈다. Active Skill로 나누어야 한다.
		// IceArrow / FireArrow 이렇게 관리되어야 한다.

		//Normal 제외 공격들.
		if (_isUltimateAttackingNow)
		{
			UpdateExecuteUltimateAttack();
		}
		else
		{
			if (_isStrongAttackingNow)
			{
				UpdateExecuteStrongAttack();
			}
			else
			{
				if (_isIceAttackingNow)
				{
					//Ice Shooting. Sound / float타임 교체해야.
					ExecuteSpecificArrowShoot(&_iceArrowVec, _playerHandler->_iceSkillAudio, _normal_timeSinceLastShot);
					//단발성.
					_isIceAttackingNow = false;
				}
				if (_isFireAttackingNow)
				{
					//Fire Shooting. Sound / float타임 교체해야.
					ExecuteSpecificArrowShoot(&_fireArrowVec, _playerHandler->_fireSkillAudio, _normal_timeSinceLastShot);
					_isFireAttackingNow = false;
				}
				else
				{
					//
				}

			}
		}

		//궁극기 쏠 때는 일반공격 막아야함.
		if (!_ulArrowLogic->GetUltimateSkillEnd())
		{
			NormalArrowShootingLogic();
		}
	}

	void PlayerCombatSector::NormalArrowShootingLogic()
	{
		//움직여도 시간 자체는 충전되어야 한다.
		// 경과 시간을 누적
		_normal_timeSinceLastShot += _pgTime->GetDeltaTime();

		// 쿨다운이 아직 남아 있으면 리턴
		if (_normal_timeSinceLastShot < _normal_shootCooldown)
		{
			return;
		}

		if (!(_playerHandler->GetPlayerMovementSector()->GetIsMoving()))
		{
			//마우스 좌클릭 시 공격.
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
			{
				//공격 애니매이션
				int animNum = _comboSystem->GetComboCount() + 4;
				if (_comboSystem->GetComboCount() == 0) animNum++;

				_playerHandler->_meshRenderer->SetAnimation("PA_0000" + std::to_string(animNum) + ".pganim", false);

				//Normal Arrow Shooting.
				ExecuteSpecificArrowShoot(&_normalArrowVec, _playerHandler->_commonAttackAudio, _normal_timeSinceLastShot);
			}
		}
		else
		{
			return;
		}
	}

	bool PlayerCombatSector::CheckActivateUltimateAttack()
	{

		//들어왔다는 것은 궁극기가 쓰일 수 있다는 뜻.
		//가장 우선권을 가지고 있다.

		// ManaPoint가 10보다 크거나 같음.
		if (CheckUltimateAttack())
		{
			PG_ERROR("ActivateUltimateAttack");

			//마나 사용. Clamp라 0으로 될 것이다.
			_playerHandler->ChangePlayerMana(-ULTIMATE_ATTACK_REQUIRED_MANA);

			_isUltimateAttackingNow = true;
			_isJustUltimateAttackInvoked = true; // 멀티 프레임 공격이니, 막 발동되었다는 의미로.
			return true;
		}
		else
		{
			_isUltimateAttackingNow = false;
			return false;
		}
	}

	bool PlayerCombatSector::CheckActivateStrongAttack()
	{

		//1칸 이상은 있어야 발동될 수 있을 것.
		if (CheckStrongAttack())
		{
			PG_ERROR("ActivateStrongAttack");

			//스태미너 1칸 사용 (5칸 중에)
			_playerHandler->ChangePlayerStamina(-STRONG_ATTACK_REQUIRED_STAMINA);

			_isStrongAttackingNow = true;
			_isJustStrongAttackInvoked = true; // 멀티 프레임 공격이니, 막 발동되었다는 의미로.
			return true; //강공격 가능.
		}
		else
		{
			_isStrongAttackingNow = false;
			return false; //강공격 불가.
		}
	}

	bool PlayerCombatSector::CheckActivateFireAttack()
	{
		if (CheckFireAttack())
		{
			PG_ERROR("ActivateFireAttack");

			//마나 한칸 사용.
			_playerHandler->ChangePlayerMana(-FIRE_ATTACK_REQUIRED_MANA);

			_isFireAttackingNow = true;
			//_isIceAttackingNow = false; //상대도 취소 하지 않음.
			return true;
		}
		else
		{
			_isFireAttackingNow = false;
			return false;
		}
	}

	bool PlayerCombatSector::CheckActivateIceAttack()
	{
		if (CheckIceAttack())
		{
			PG_ERROR("ActivateIceAttack");

			//마나 한칸 사용.
			_playerHandler->ChangePlayerMana(-ICE_ATTACK_REQUIRED_MANA);

			_isIceAttackingNow = true;
			//_isFireAttackingNow = false; //상대도 취소 하지 않음.
			return true;
		}
		else
		{
			_isIceAttackingNow = false;
			return false;
		}
	}

	void PlayerCombatSector::UpdateExecuteUltimateAttack()
	{
		//반복적으로 들어올 것이다.
		if (_isJustUltimateAttackInvoked)
		{
			//Ultimate Invoke 부분이 여기로!
			InvokeSingleUltimateAttack();
			_isJustUltimateAttackInvoked = false;
		}

		//시간을 세든, 끝날 때의 신호를 받든해서 더 이상 멀티프레임 공격을 실행중이지 않다는 것 알려야.
		//여러 프레임이 걸쳐 이루어지는 로직. -> 시간을 세는 방식.
		if (!_ulArrowLogic->GetUltimateSkillEnd())
		{
			//더 이상은 호출되지 않는다. Ultimate 관련된 모든거 다 리셋.
			_isJustUltimateAttackInvoked = false;
			_isUltimateAttackingNow = false;
		}
	}

	void PlayerCombatSector::UpdateExecuteStrongAttack()
	{
		//반복적으로 들어올 것이다.
		if (_isJustStrongAttackInvoked)
		{
			//Strong Invoke 부분이 여기로! Strong 관련된 모든거 다 리셋.
			InvokeSingleStrongAttack();
			_isJustStrongAttackInvoked = false;
			_startedStrongAttackingTime = 0.f;

			_playerHandler->GetPlayerMovementSector()->SetUseStrongSkill(true);
		}

		//시간을 세든, 끝날 때의 신호를 받든해서 더 이상 멀티프레임 공격을 실행중이지 않다는 것 알려야.
		//여러 프레임이 걸쳐 이루어지는 로직. -> 시간을 세는 방식.
		_startedStrongAttackingTime += _pgTime->GetDeltaTime();
		if (_startedStrongAttackingTime >= STRONG_ATTACK_DURATION)
		{
			//이제 Strong Attack 비활성화
			_strongAttackLogic->Deactivate();

			//더 이상은 호출되지 않는다.
			_startedStrongAttackingTime = 0.f;
			_isJustStrongAttackInvoked = false;
			_isStrongAttackingNow = false;

			_playerHandler->GetPlayerMovementSector()->SetUseStrongSkill(false);
		}
	}


	void PlayerCombatSector::ExecuteSpecificArrowShoot(std::vector<ArrowLogic*>* typeArrowVec, Pg::Data::AudioSource* audioSource, float& outIfDoneResetTime)
	{
		//ComboCounting은 ComboSystem에서 회수해야 한다.

		bool tDidShoot = false;

		for (int i = 0; i < typeArrowVec->size(); i++)
		{
			//지금 쏘고 있지 않은 컴포넌트만 허용해야.
			if (!(typeArrowVec->at(i)->GetIsNowShooting()))
			{
				using namespace Pg::Math;
				//우리 Forward랑 다른 로직이 된 것 같다. 그러니, Forward를 Rotation을 갖고 Custom으로 구해주자.
				//PlayerBehavior랑 같은 위치. -> 나중에 PhysX 연동은 고쳐져야!

				//시작점 역시 Offset 하기로 했었다.
				Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position + Pg::Math::PGFLOAT3(0.f, 1.f, 0.f);
				//Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position;


				//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
				Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(-_object->_transform.GetForward(), { 0,0,1 });
				tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

				//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());

				float tDistanceToSpawnFrom = 0.01f;
				//_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
				typeArrowVec->at(i)->ShootArrow(tStartingPosition + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
				tDidShoot = true;

				//쿨타임 초기화
				outIfDoneResetTime = 0.0f;
				break;
			}

		}

		if (!tDidShoot)
		{
			//쐈어야 하는데 기존의 것 중에서 준비되어 있는 오브젝트가 존재하지 않는다!
			//기존 것 중에서 오래된 요소를 찾아서, (FixedSizeQueue) 여기 기준으로 마지막을 원래대로 돌려야 한다.
			//여튼, 그건 나중에 할 일.
			PG_TRACE("아직 충분히 반환되지 않음. 나중에 FixedSizeQueue로?");
		}

		audioSource->Play();
	}

	void PlayerCombatSector::InvokeSingleUltimateAttack()
	{
		//궁극기 발동 로직. 이렇게 켜주는 거고, 내부적으로 알아서 민서가 꺼줘야 함 (onAnimationEnd에 맞춰서)
		//_ulArrowCol->SetActive(true); //충돌 키는 용.
		//_ulArrowLogic->_isSkillStart = true; //로직 키는 용.
		_ulArrowLogic->StartSkill();

		_playerHandler->_meshRenderer->SetAnimation("PA_00011.pganim", false);
	}

	void PlayerCombatSector::InvokeSingleStrongAttack()
	{
		_strongAttackLogic->Activate(&(_playerHandler->_object->_transform._position));

		_playerHandler->_meshRenderer->SetAnimation("PA_00008.pganim", false);
	}

	bool PlayerCombatSector::CheckStrongAttack()
	{
		return ((_playerHandler->GetIsStaminaReadyToUse()) && (!_isUltimateAttackingNow)
			&& (_playerHandler->staminaPoint >= STRONG_ATTACK_REQUIRED_STAMINA));
	}

	bool PlayerCombatSector::CheckUltimateAttack()
	{
		return (_playerHandler->manaPoint >= ULTIMATE_ATTACK_REQUIRED_MANA);
	}

	bool PlayerCombatSector::CheckFireAttack()
	{
		return (!_isUltimateAttackingNow)
			&& (!_isStrongAttackingNow)
			&& (_playerHandler->manaPoint >= FIRE_ATTACK_REQUIRED_MANA);
	}

	bool PlayerCombatSector::CheckIceAttack()
	{
		return (!_isUltimateAttackingNow)
			&& (!_isStrongAttackingNow)
			&& (_playerHandler->manaPoint >= ICE_ATTACK_REQUIRED_MANA);
	}

	void PlayerCombatSector::UpdateForGUIVariables()
	{
		_checkUltimateAttack = CheckUltimateAttack();
		_checkStrongAttack = CheckStrongAttack();
		_checkFireAttack = CheckFireAttack();
		_checkIceAttack = CheckIceAttack();
	}

	void PlayerCombatSector::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		if (justEndedAnimation == "PA_00005.pganim" ||
			justEndedAnimation == "PA_00006.pganim" ||
			justEndedAnimation == "PA_00007.pganim" ||
			justEndedAnimation == "PA_00008.pganim" ||
			justEndedAnimation == "PA_00009.pganim" ||
			justEndedAnimation == "PA_000011.pganim" )
		{
			_playerHandler->_meshRenderer->SetAnimation("PA_00015.pganim", false);
			_isWaiting = true;
		}
	}
}