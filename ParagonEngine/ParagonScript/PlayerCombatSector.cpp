#include "PlayerCombatSector.h"
#include "../ParagonData/Scene.h"
#include "ArrowLogic.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"
#include "CombatSystem.h"
#include "EventList_PlayerRelated.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"

#include <singleton-cpp/singleton.h>

#include "../ParagonData/LayerMask.h"

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
	}

	void PlayerCombatSector::BeforePhysicsAwake()
	{

	}

	void PlayerCombatSector::Awake()
	{

	}

	void PlayerCombatSector::Start()
	{

	}

	void PlayerCombatSector::Update()
	{
		SelectActivateActiveSkill();
		ProcessInputsForStrongAttack();
		ProcessInputsForUltimateAttack();
		AllAttacksLogic();
		//나머지 로직은 Combat System으로 이동.
	}

	void PlayerCombatSector::FixedUpdate()
	{

	}

	void PlayerCombatSector::LateUpdate()
	{
		PlayAdequateAnimation();
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

		//현재 공격 상태 리셋.
		_isStrongAttackingNow = false;
		_isUltimateAttackingNow = false;

		//UI Manager : 내부 액티브스킬 GUI 초기 세팅 따로 해야 한다.
	}

	void PlayerCombatSector::PlayAdequateAnimation()
	{
		//우선, 디폴트로 출력되는 것은 Idle Animation. 

		//Idle 초기 상태 세팅.
		std::string tToPlayAnimationName = "PA_00001.pganim";
		bool isLooping = true;

		if (_isHit)
		{
			//공격 애니매이션
			isLooping = false;
			tToPlayAnimationName = "PA_0000" + std::to_string(_hitCount + 4) + ".pganim";
		}

		//만약에 전 스트링과 같지 않을 시에.
		if (_prevAnimationInput.compare(tToPlayAnimationName) != 0)
		{
			_playerHandler->_meshRenderer->SetAnimation(tToPlayAnimationName, isLooping);
		}

		//애니메이션 인풋 스트링 기록.
		_prevAnimationInput = tToPlayAnimationName;
	}

	void PlayerCombatSector::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		// Loop가 안되는 모든 애니매이션의 flag는 여기서 false로 변경
		_isHit = false;
	}

	void PlayerCombatSector::SelectActivateActiveSkill()
	{
		// 일반 공격 : 그냥 지금처럼 화살 
		// 액티브면 불 / 얼음 화살 이렇게 스위칭이고,
		// Q - E로 바꾸면 즉시 화살 쏘는 것처럼 투사체 나감.
		//Q와 E를 누르면 Switch.
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyQ))
		{
			//Event HandleEvents에서 나중에 구분해야 한다.
			_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), false, NULL);
			ActivateFireAttack();
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyE))
		{
			//Event HandleEvents에서 나중에 구분해야 한다.
			_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), true, NULL);
			ActivateIceAttack();
		}
	}

	void PlayerCombatSector::ProcessInputsForStrongAttack()
	{
		//마우스 클릭한 순간부터 기록 시작.

		//만약 강공격을 쓸 수 있는 상태라면
		if (_isStrongAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
			{
				_startedClickingTime = 0.f;
				//PG_WARN("STARTING_COUNTING");
			}

			if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MouseLeft))
			{
				//다시 클릭되기까지 쓰이지 않을 것.
				_startedClickingTime = 0.f;
				//PG_WARN("ENDING_COUNTING");
			}

			if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MouseLeft))
			{
				_startedClickingTime += _pgTime->GetDeltaTime();

				//만약 2초보다 더 길게 클릭한다면 -> 강공격 발동.
				if (_startedClickingTime >= 2.0f)
				{
					//PG_ERROR("Counting : {0}", _startedClickingTime);
					ActivateStrongAttack();
					_startedClickingTime = 0.f;
					_startedStrongAttackChargeTime = 0.f;
					_isStrongAttackStartEligible = false;
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
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyF))
			{
				// ManaPoint가 10보다 크거나 같음.
				if (_playerHandler->manaPoint >= ULTIMATE_ATTACK_REQUIRED_MANA)
				{
					_playerHandler->ChangePlayerMana(-100000.f);
					
					ActivateUltimateAttack();
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
				//Arrow에 자신 할당.
				tALogic->_playerBattleBehavior = _playerHandler;
				_arrowVec.push_back(tALogic);
			}
		}
	}

	void PlayerCombatSector::AllAttacksLogic()
	{
		// 여기서 불 / 얼음을 나눈다. Active Skill로 나누어야 한다.
		ArrowShootingLogic();


	}

	void PlayerCombatSector::ArrowShootingLogic()
	{
		if (_playerHandler->GetPlayerMovementSector()->GetIsMoving() == false)
		{
			// 경과 시간을 누적
			_timeSinceLastShot += _pgTime->GetDeltaTime();

			// 쿨다운이 아직 남아 있으면 리턴
			if (_timeSinceLastShot < _shootCooldown)
			{
				return;
			}

			//마우스 좌클릭 시 공격.
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
			{
				_isHit = true;
				_hitCount++;
				if (_hitCount >= 4) _hitCount = 1;

				bool tDidShoot = false;

				for (int i = 0; i < _arrowVec.size(); i++)
				{
					//지금 쏘고 있지 않은 컴포넌트만 허용해야.
					if (!(_arrowVec[i]->GetIsNowShooting()))
					{
						using namespace Pg::Math;
						//우리 Forward랑 다른 로직이 된 것 같다. 그러니, Forward를 Rotation을 갖고 Custom으로 구해주자.
						//PlayerBehavior랑 같은 위치. -> 나중에 PhysX 연동은 고쳐져야!

						//시작점 역시 Offset 하기로 했었다.
						Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position + Pg::Math::PGFLOAT3(0.f, 2.f, 0.f);


						//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
						Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(-_object->_transform.GetForward(), { 0,0,1 });
						tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

						//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());

						float tDistanceToSpawnFrom = 0.01f;
						//_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
						_arrowVec[i]->ShootArrow(tStartingPosition + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
						tDidShoot = true;

						//쿨타임 초기화
						_timeSinceLastShot = 0.0f;
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

				_playerHandler->_commonAttackAudio->Play();
			}
		}
		else
		{
			_hitCount = 0;
			return;
		}
	}

	void PlayerCombatSector::ActivateStrongAttack()
	{
		PG_ERROR("ActivateStrongAttack");
	}

	void PlayerCombatSector::ActivateUltimateAttack()
	{
		PG_ERROR("ActivateUltimateAttack");
	}

	void PlayerCombatSector::ActivateFireAttack()
	{
		PG_ERROR("ActivateFireAttack");
	}

	void PlayerCombatSector::ActivateIceAttack()
	{
		PG_ERROR("ActivateIceAttack");
	}

}