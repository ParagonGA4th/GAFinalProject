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

		//�ñر� ���� ��� ��� ã��.
		tUltimateArrow = _object->GetScene()->FindObjectWithName("UltimateArrow");
		assert(tUltimateArrow != nullptr);
		_ulArrowLogic = tUltimateArrow->GetComponent<UltimateArrowLogic>();
		assert(_ulArrowLogic != nullptr);


	}

	void PlayerCombatSector::Awake()
	{
		if (_playerHandler->_object->GetScene()->GetSceneName() == "BossStage")
		{
			auto _boss = _playerHandler->_object->GetScene()->FindObjectWithName("Boss");
			//assert(_boss != nullptr);
			_bossBehaviour = _boss->GetComponent<BossBehaviour>();
			//assert(_bossBehaviour != nullptr);
		}
	}

	void PlayerCombatSector::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void PlayerCombatSector::Update()
	{
		//������ �� ������ �̹� ��������.
		if (_bossBehaviour != nullptr)
		{
			if (_bossBehaviour->GetProhibitAttack() == false)
			{
				ProcessInputsForActiveSkills();
				ProcessInputsForUltimateAttack();
				ProcessInputsForStrongAttack();
				UpdateForGUIVariables();
				AllAttacksLogic();
				//������ ������ Combat System���� �̵�.

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
		}
		else
		{
			ProcessInputsForActiveSkills();
			ProcessInputsForUltimateAttack();
			ProcessInputsForStrongAttack();
			UpdateForGUIVariables();
			AllAttacksLogic();
			//������ ������ Combat System���� �̵�.

		ProcessInputsForActiveSkills();
		ProcessInputsForUltimateAttack();
		ProcessInputsForStrongAttack();
		UpdateForGUIVariables();
		AllAttacksLogic();
		//������ ������ Combat System���� �̵�.

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
		// ���� ���� ������ ����.
		_startedClickingTime = 0.f;
		_isStrongAttackStartEligible = true;
		_startedStrongAttackChargeTime = 0.f;

		_isUltimateAttackStartEligible = true;
		_isStartedUltimateAttackChargeTime = 0.f;

		_isIceAttackStartEligible = true;
		_isFireAttackStartEligible = true;

		_isStartedIceSkillChargeTime = 0.f;
		_isStartedFireSkillChargeTime = 0.f;

		//���� ���� ���� ����.
		_isStrongAttackingNow = false;
		_isUltimateAttackingNow = false;
		_isIceAttackingNow = false;
		_isFireAttackingNow = false;

		//�ܹ߼� �ƴ� ���ݵ� ���� ����.
		_isJustStrongAttackInvoked = false;
		_isJustUltimateAttackInvoked = false;

		_startedStrongAttackingTime = 0.f;

		//UI Manager : ���� ��Ƽ�꽺ų GUI �ʱ� ���� ���� �ؾ� �Ѵ�.
	}

	void PlayerCombatSector::ProcessInputsForActiveSkills()
	{
		// �Ϲ� ���� : �׳� ����ó�� ȭ�� 
		// ��Ƽ��� �� / ���� ȭ�� �̷��� ����Ī�̰�,
		// Q - E�� �ٲٸ� ��� ȭ�� ��� ��ó�� ����ü ����.
		//Q�� E�� ������ Switch.

		if (_isFireAttackStartEligible)
		{
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyQ))
			{
				if (CheckActivateFireAttack())
				{
					//Event HandleEvents���� ���߿� �����ؾ� �Ѵ�.
					_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), false, NULL);
					//���� Cooldown ����.
					_isFireAttackStartEligible = false;
					_isStartedFireSkillChargeTime = 0.f;

					_playerHandler->_meshRenderer->SetAnimation("PA_00009.pganim", false);
				}
			}
		}
		else
		{
			//��ٿ� ����.
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
					//Event HandleEvents���� ���߿� �����ؾ� �Ѵ�.
					_playerHandler->_combatSystem->Post(Event_UI_SetActiveSkill(), true, NULL);
					//���� Cooldown ����.
					_isIceAttackStartEligible = false;
					_isStartedIceSkillChargeTime = 0.f;

					_playerHandler->_meshRenderer->SetAnimation("PA_00009.pganim", false);
				}
			}
		}
		else
		{
			//��ٿ� ����.
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
		//���콺 Ŭ���� �������� ��� ����.

		//���� �������� �� �� �ִ� ���¶��
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

				//���� 2�ʺ��� �� ��� Ŭ���Ѵٸ� -> ������ �ߵ�.
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
			// �������� ���� �Ұ����ϴٸ�, �ٽ� ��ٿ� �������� ���� �������� �ʿ��ϴ�.
			_startedStrongAttackChargeTime += _pgTime->GetDeltaTime();
			//3�� ��ٿ� �ʿ�.
			if (_startedStrongAttackChargeTime >= STRONG_ATTACK_COOLDOWN_TIME)
			{
				//PG_ERROR("COOLDOWNDONE");
				//�ٽ� ȣ��� �� �ְ�.
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
				//�ñر� �����ϸ�
				if (CheckActivateUltimateAttack())
				{
					// 0���� �ٽ� ��������. ��ٿ��� ����.
					_isStartedUltimateAttackChargeTime = 0.f;
					_isUltimateAttackStartEligible = false;
				}
			}
		}
		else
		{
			//�ƴϸ�, �ñر��� ���� ��ٿ� �������� �ؾ� �Ѵ�.
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
		//ArrowLogic�� Awake���� �ڽ��� Tag�� �̹� "TAG_Arrow"�� �����س����� ���̴�.
		//��� Arrow�� �޾ƿ���. �� 30���� �Ǿ�� ��ƴ�� ��ġ�� �� ç ���̴�.
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
				else { assert(false && "����Ʈ ���� �� Ÿ����."); }
			}
		}
		assert("");
		//if (_iceArrowVec.empty() ||

		//Material���� �ٸ��� ������� �Ѵ�.
		//UltimateArrowAttack�� ������ �ڽ� ��ü�� �� ��.
		//IceArrow : 15�� / FireArrow : 15�� / �Ϲ� Arrow : 20��.
	}

	void PlayerCombatSector::AllAttacksLogic()
	{
		// ���⼭ �� / ������ ������. Active Skill�� ������� �Ѵ�.
		// IceArrow / FireArrow �̷��� �����Ǿ�� �Ѵ�.

		//Normal ���� ���ݵ�.
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
					//Ice Shooting. Sound / floatŸ�� ��ü�ؾ�.
					ExecuteSpecificArrowShoot(&_iceArrowVec, _playerHandler->_iceSkillAudio, _normal_timeSinceLastShot);
					//�ܹ߼�.
					_isIceAttackingNow = false;
				}
				if (_isFireAttackingNow)
				{
					//Fire Shooting. Sound / floatŸ�� ��ü�ؾ�.
					ExecuteSpecificArrowShoot(&_fireArrowVec, _playerHandler->_fireSkillAudio, _normal_timeSinceLastShot);
					_isFireAttackingNow = false;
				}
				else
				{
					//
				}

			}
		}

		//�ñر� �� ���� �Ϲݰ��� ���ƾ���.
		if (!_ulArrowLogic->GetUltimateSkillEnd())
		{
			NormalArrowShootingLogic();
		}
	}

	void PlayerCombatSector::NormalArrowShootingLogic()
	{
		//�������� �ð� ��ü�� �����Ǿ�� �Ѵ�.
		// ��� �ð��� ����
		_normal_timeSinceLastShot += _pgTime->GetDeltaTime();

		// ��ٿ��� ���� ���� ������ ����
		if (_normal_timeSinceLastShot < _normal_shootCooldown)
		{
			return;
		}

		if (!(_playerHandler->GetPlayerMovementSector()->GetIsMoving()))
		{
			//���콺 ��Ŭ�� �� ����.
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
			{
				//���� �ִϸ��̼�
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

		//���Դٴ� ���� �ñرⰡ ���� �� �ִٴ� ��.
		//���� �켱���� ������ �ִ�.

		// ManaPoint�� 10���� ũ�ų� ����.
		if (CheckUltimateAttack())
		{
			PG_ERROR("ActivateUltimateAttack");

			//���� ���. Clamp�� 0���� �� ���̴�.
			_playerHandler->ChangePlayerMana(-ULTIMATE_ATTACK_REQUIRED_MANA);

			_isUltimateAttackingNow = true;
			_isJustUltimateAttackInvoked = true; // ��Ƽ ������ �����̴�, �� �ߵ��Ǿ��ٴ� �ǹ̷�.
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

		//1ĭ �̻��� �־�� �ߵ��� �� ���� ��.
		if (CheckStrongAttack())
		{
			PG_ERROR("ActivateStrongAttack");

			//���¹̳� 1ĭ ��� (5ĭ �߿�)
			_playerHandler->ChangePlayerStamina(-STRONG_ATTACK_REQUIRED_STAMINA);

			_isStrongAttackingNow = true;
			_isJustStrongAttackInvoked = true; // ��Ƽ ������ �����̴�, �� �ߵ��Ǿ��ٴ� �ǹ̷�.
			return true; //������ ����.
		}
		else
		{
			_isStrongAttackingNow = false;
			return false; //������ �Ұ�.
		}
	}

	bool PlayerCombatSector::CheckActivateFireAttack()
	{
		if (CheckFireAttack())
		{
			PG_ERROR("ActivateFireAttack");

			//���� ��ĭ ���.
			_playerHandler->ChangePlayerMana(-FIRE_ATTACK_REQUIRED_MANA);

			_isFireAttackingNow = true;
			//_isIceAttackingNow = false; //��뵵 ��� ���� ����.
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

			//���� ��ĭ ���.
			_playerHandler->ChangePlayerMana(-ICE_ATTACK_REQUIRED_MANA);

			_isIceAttackingNow = true;
			//_isFireAttackingNow = false; //��뵵 ��� ���� ����.
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
		//�ݺ������� ���� ���̴�.
		if (_isJustUltimateAttackInvoked)
		{
			//Ultimate Invoke �κ��� �����!
			InvokeSingleUltimateAttack();
			_isJustUltimateAttackInvoked = false;
		}

		//�ð��� ����, ���� ���� ��ȣ�� �޵��ؼ� �� �̻� ��Ƽ������ ������ ���������� �ʴٴ� �� �˷���.
		//���� �������� ���� �̷������ ����. -> �ð��� ���� ���.
		if (!_ulArrowLogic->GetUltimateSkillEnd())
		{
			//�� �̻��� ȣ����� �ʴ´�. Ultimate ���õ� ���� �� ����.
			_isJustUltimateAttackInvoked = false;
			_isUltimateAttackingNow = false;
		}
	}

	void PlayerCombatSector::UpdateExecuteStrongAttack()
	{
		//�ݺ������� ���� ���̴�.
		if (_isJustStrongAttackInvoked)
		{
			//Strong Invoke �κ��� �����! Strong ���õ� ���� �� ����.
			InvokeSingleStrongAttack();
			_isJustStrongAttackInvoked = false;
			_startedStrongAttackingTime = 0.f;

			_playerHandler->GetPlayerMovementSector()->SetUseStrongSkill(true);
		}

		//�ð��� ����, ���� ���� ��ȣ�� �޵��ؼ� �� �̻� ��Ƽ������ ������ ���������� �ʴٴ� �� �˷���.
		//���� �������� ���� �̷������ ����. -> �ð��� ���� ���.
		_startedStrongAttackingTime += _pgTime->GetDeltaTime();
		if (_startedStrongAttackingTime >= STRONG_ATTACK_DURATION)
		{
			//���� Strong Attack ��Ȱ��ȭ
			_strongAttackLogic->Deactivate();

			//�� �̻��� ȣ����� �ʴ´�.
			_startedStrongAttackingTime = 0.f;
			_isJustStrongAttackInvoked = false;
			_isStrongAttackingNow = false;

			_playerHandler->GetPlayerMovementSector()->SetUseStrongSkill(false);
		}
	}


	void PlayerCombatSector::ExecuteSpecificArrowShoot(std::vector<ArrowLogic*>* typeArrowVec, Pg::Data::AudioSource* audioSource, float& outIfDoneResetTime)
	{
		//ComboCounting�� ComboSystem���� ȸ���ؾ� �Ѵ�.

		bool tDidShoot = false;

		for (int i = 0; i < typeArrowVec->size(); i++)
		{
			//���� ��� ���� ���� ������Ʈ�� ����ؾ�.
			if (!(typeArrowVec->at(i)->GetIsNowShooting()))
			{
				using namespace Pg::Math;
				//�츮 Forward�� �ٸ� ������ �� �� ����. �׷���, Forward�� Rotation�� ���� Custom���� ��������.
				//PlayerBehavior�� ���� ��ġ. -> ���߿� PhysX ������ ��������!

				//������ ���� Offset �ϱ�� �߾���.
				Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position + Pg::Math::PGFLOAT3(0.f, 1.f, 0.f);
				//Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position;


				//Z�� ���� ������. ��𿡼� �ҿ����� ������ �Ͼ������ Ȯ���غ��� �� �� ����.
				Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(-_object->_transform.GetForward(), { 0,0,1 });
				tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

				//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());

				float tDistanceToSpawnFrom = 0.01f;
				//_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
				typeArrowVec->at(i)->ShootArrow(tStartingPosition + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
				tDidShoot = true;

				//��Ÿ�� �ʱ�ȭ
				outIfDoneResetTime = 0.0f;
				break;
			}

		}

		if (!tDidShoot)
		{
			//����� �ϴµ� ������ �� �߿��� �غ�Ǿ� �ִ� ������Ʈ�� �������� �ʴ´�!
			//���� �� �߿��� ������ ��Ҹ� ã�Ƽ�, (FixedSizeQueue) ���� �������� �������� ������� ������ �Ѵ�.
			//��ư, �װ� ���߿� �� ��.
			PG_TRACE("���� ����� ��ȯ���� ����. ���߿� FixedSizeQueue��?");
		}

		audioSource->Play();
	}

	void PlayerCombatSector::InvokeSingleUltimateAttack()
	{
		//�ñر� �ߵ� ����. �̷��� ���ִ� �Ű�, ���������� �˾Ƽ� �μ��� ����� �� (onAnimationEnd�� ���缭)
		//_ulArrowCol->SetActive(true); //�浹 Ű�� ��.
		//_ulArrowLogic->_isSkillStart = true; //���� Ű�� ��.
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
			justEndedAnimation == "PA_000011.pganim")
		{
			_playerHandler->_meshRenderer->SetAnimation("PA_00015.pganim", false);
			_isWaiting = true;
		}
	}
}