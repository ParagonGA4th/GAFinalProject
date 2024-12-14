#include "BossBehaviour.h"
#include "CombatSystem.h"
#include "EventList_GameFlowRelated.h"
#include "CameraShake.h"
#include "BaseEnemyHandler.h"
#include "TotalGameManager.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"

#include "../ParagonAPI/PgTween.h"
#include "../ParagonUtil/Tween.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	BossBehaviour::BossBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();

		//���� ü�°� ����
		_bossInfo = new BossInfo(MAX_BOSS_HEALTH, BOSS_ATTACK_POWER);
		//_bossInfo = new BossInfo(2.f, 4.f);

		///������ ��� �� �ǰ��ൿ�� CombatSystem���� ������ �޺��� ��ų�� ����
		///���Ϳ��� ���������� �����ϱ⿡ ���⼭�� ��� �� �ൿ�� ����� �ȴ�.
		_bossInfo->_onDead = [this]() { Dead(); };

		_bossInfo->_onHit = [this]() { Hit(); };
	}

	void BossBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void BossBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void BossBehaviour::GrabManagedObjects()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_BOSS);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		//�÷��̾� ����
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		auto bossWalkSound = _object->GetScene()->FindObjectWithName("BossWalkSound");
		_walkAudio = bossWalkSound->GetComponent<Pg::Data::AudioSource>();

		auto bossRushSound = _object->GetScene()->FindObjectWithName("BossRushSound");
		_rushAudio = bossRushSound->GetComponent<Pg::Data::AudioSource>();

		auto bossDieSound = _object->GetScene()->FindObjectWithName("BossDieSound");
		_dieAudio = bossDieSound->GetComponent<Pg::Data::AudioSource>();

		auto basicAttackSound1 = _object->GetScene()->FindObjectWithName("BossAttackSound1");
		_basicAttackAudio1 = basicAttackSound1->GetComponent<Pg::Data::AudioSource>();

		auto basicAttackSound2 = _object->GetScene()->FindObjectWithName("BossAttackSound2");
		_basicAttackAudio2 = basicAttackSound2->GetComponent<Pg::Data::AudioSource>();		
		
		auto laserSound = _object->GetScene()->FindObjectWithName("BossLaserSound");
		_laserAttack = laserSound->GetComponent<Pg::Data::AudioSource>();
		
		auto upSound = _object->GetScene()->FindObjectWithName("BossUpSound");
		_upSound = upSound->GetComponent<Pg::Data::AudioSource>();

		auto downSound = _object->GetScene()->FindObjectWithName("BossDownSound");
		_downAudio = downSound->GetComponent<Pg::Data::AudioSource>();

		auto hit = _object->GetScene()->FindObjectWithName("BossHitSound");
		_hitAudio = hit->GetComponent<Pg::Data::AudioSource>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_meshRenderer->SetRendererOffset(_rendererOffset);
		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		_wind = _object->GetScene()->FindObjectWithName("BossWindBlastEffect");
		_windRenderer = _wind->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_WindBlast")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_windBlastAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
			else if (childTag == "TAG_TakeDown")
			{
				Pg::Data::StaticSphereCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticSphereCollider>();
				if (skillStaticCol != nullptr)
				{
					_takeDownCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}

		for (auto& iter : _object->GetScene()->FindObjectsWithTag("TAG_Light"))
		{
			Pg::Data::StaticBoxCollider* skillStaticCol = iter->GetComponent<Pg::Data::StaticBoxCollider>();
			Pg::Data::SkinnedMeshRenderer* skillRenderer = iter->GetComponent<Pg::Data::SkinnedMeshRenderer>();

			if (skillStaticCol != nullptr)
			{
				_lightAttackCol.push_back(skillStaticCol);
				skillStaticCol->SetActive(false);
			}
			if (skillRenderer != nullptr)
			{
				_lightSkillRenderer.push_back(skillRenderer);
				skillRenderer->SetAlphaPercentage(0.f);
			}
		}
	}

	void BossBehaviour::BeforePhysicsAwake()
	{
		//_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_BOSS);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer->SetRendererOffset(_rendererOffset);

		_windRenderer->SetAnimation("boss_effect_0.pganim", false);
		_windRenderer->PauseAnim();
		_windRenderer->SetAlphaPercentage(0.f);

		//clear �ʿ���.
		if (!_basicAttackCol.empty() || !_windBlastAttackCol.empty() || !_lightAttackCol.empty()
			|| !_takeDownCol.empty())
		{
			_basicAttackCol.clear();
			_windBlastAttackCol.clear();
			_lightAttackCol.clear();
			_takeDownCol.clear();
		}

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_WindBlast")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_windBlastAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
			else if (childTag == "TAG_TakeDown")
			{
				Pg::Data::StaticSphereCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticSphereCollider>();
				if (skillStaticCol != nullptr)
				{
					_takeDownCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}

		for (auto& iter : _object->GetScene()->FindObjectsWithTag("TAG_Light"))
		{
			Pg::Data::StaticBoxCollider* skillStaticCol = iter->GetComponent<Pg::Data::StaticBoxCollider>();
			Pg::Data::SkinnedMeshRenderer* skillRenderer = iter->GetComponent<Pg::Data::SkinnedMeshRenderer>();

			if (skillStaticCol != nullptr)
			{
				_lightAttackCol.push_back(skillStaticCol);
				skillStaticCol->SetActive(false);
			}
			if (skillRenderer != nullptr)
			{
				_lightSkillRenderer.push_back(skillRenderer);
				skillRenderer->SetAlphaPercentage(0.f);
			}
		}
	}

	void BossBehaviour::Awake()
	{
		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}

		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void BossBehaviour::Start()
	{
		_bossInfo->StartBaseMonsterLogic();
	}

	void BossBehaviour::Update()
	{
		//PG_TRACE(_monsterHelper->_bossFlag._bossStateListByEnum[_monsterHelper->_bossFlag._bossState]);
		PG_TRACE(_meshRenderer->GetAnimation());

		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;

			//�� ����.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast���� �����ִ� Collider�� �˻簡 �Ǳ� ������, ������ ������ ������ ��ġ�� �����ش�.
			_object->_transform._position = { 0, -1000, 0 };

			//������ �̰�ٰ� (==Boss �׿��ٰ� Event Post) �˸���!
			_combatSystem->Post(Event_OnFinalBossDeathGameWin(), NULL, NULL);
		}

		if (_isRotatingToPlayer) RotateToPlayer(_playerTransform->_position);
		if (_distance <= _bossInfo->GetSightRange() && !_isPlayerInit)
		{
			_monsterHelper->_isPlayerDetected = true;

			if (_dashCount >= 3)
			{
				_isPlayerInit = true;
				_monsterHelper->_bossFlag._bossState = Data::BossState::IDLE;
				_monsterHelper->_bossFlag._bossPase = Data::BossPase::PASE_1;
			}
			else
			{
				_monsterHelper->_bossFlag._bossState = Data::BossState::DASH;
				Dash();
			}

			return;
		}

		Neutralize();
		if (_monsterHelper->_bossFlag._bossState == Data::BossState::DOWN) return;

		if (_monsterHelper->_bossFlag._bossState == Data::BossState::DASH)
		{
			Dash();
		}
		else
		{
			if (_distance <= _bossInfo->GetAttackRange())
			{
				_meshRenderer->_animBlendFactor = 0.0f;

				_isRotatingToPlayer = true;
				_monsterHelper->_isChase = false;
				_monsterHelper->_isPlayerinHitSpace = true;			
				
				if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::BASIC_ATTACK_1 ||
					_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::BASIC_ATTACK_2)
				{
					if (!_isAttackSoundPlaying)
					{
						_basicAttackAudio1->Play();
						_isAttackSoundPlaying = true;
					}

					Attack(_monsterHelper->_isAnimChange);
					if (!_monsterHelper->_isAnimChange) _isAttackSoundPlaying = false;

					//_isRotatingToPlayer = true;
					//_useTakeDownSkill = true;
				}
				if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::BASIC_ATTACK_3)
				{
					if (!_isStormAttackSoundPlaying)
					{
						_basicAttackAudio2->Play();
						_isStormAttackSoundPlaying = true;
					}
					_isRotatingToPlayer = false;
					//Attack(false);
					//_useTakeDownSkill = false;
					_useStormBlast = true;
				}

				if (_monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3)
				{
					if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::CAST ||
						_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::CHASE)
					{
						_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::BASIC_ATTACK_1;
					}
				}
			}
			else
			{
				if (_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FEATHER_ATTACK &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FEATHER_ATTACK_PREPARE &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1 &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2 &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3 &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FLY_ATTACK_1 &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::SKILL_FLY_ATTACK_2 &&
					_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::EVASION)
				{
					//_meshRenderer->_animBlendFactor = 10.0f;

					Chase();
					Attack(false);

					_isRotatingToPlayer = true;
					_monsterHelper->_isChase = true;
					_monsterHelper->_isPlayerinHitSpace = false;
				}
			}

			if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FEATHER_ATTACK)
			{
				_isRotatingToPlayer = false;
				_useLightSkill = true;
			}
			else if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1)
			{
				_useTakeDownSkill = true;
				_useLightSkill = false;
			}
			else if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::EVASION) Evade();
		}

		//��ǳ ��ų
		UpdateSkill();
		if (_offWind)
		{
			_windRenderer->SetAlphaPercentage(_windRenderer->GetAlphaPercentage() - ALPHA_PERCENT);

			if (_windRenderer->GetAlphaPercentage() <= std::numeric_limits<float>::epsilon())
			{
				_windRenderer->SetAnimation("boss_effect_0.pganim", false);
				_windRenderer->PauseAnim();
				for (auto& iter : _windBlastAttackCol)
				{
					iter->SetActive(false);
					iter->_object->_transform._position = { 0.f, 0.f, 1.f };
				}

				_bossInfo->SetCurrentWindBlastDurationTime(0.f);
				_useStormBlast = false;
				_isRotatingToPlayer = true;
				_offWind = false;
				_isStormAttackSoundPlaying = false;
			}
		}
		//����� ��ų
		UpdatePhaseTwoSkill();

		//������� ��ų
		UpdatePhaseThreeSkill();

		_bossInfo->UpdateBaseMonsterLogic(_object);
	}

	void BossBehaviour::Chase()
	{
		//�̵� �ӵ� ����.
		float interpolation = _bossInfo->GetMoveSpeed() * _bossInfo->GetMonsterSpeedRatio() * _pgTime->GetDeltaTime();

		//���¸� Chase�� ����.

		//�ִϸ��̼�
		_monsterHelper->_isPlayerDetected = true;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = true;

		for (auto& iter : _takeDownCol)
		{
			iter->SetActive(false);
		}

		//�����Ÿ� ���̸� �÷��̾�� ��� �ٰ�����.
		///�����ϸ鼭 �̵��� �� �������� �������� ������ �߻��ϱ� ���� ����.
		Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;
		Pg::Math::PGFLOAT3 targetPosition = _playerTransform->_position;

		// ��ǥ ���������� ���� ���� ���
		Pg::Math::PGFLOAT3 direction = targetPosition - currentPosition;
		direction.y = 0; // y�� �̵��� ���� ���� y���� 0���� ����

		// ���� ���͸� ����ȭ
		Pg::Math::PGFLOAT3 directionNorm = Pg::Math::PGFloat3Normalize(direction);

		// ������ �ӵ��� �̵�
		Pg::Math::PGFLOAT3 movement = directionNorm * interpolation;

		currentPosition.x += movement.x;
		currentPosition.z += movement.z;

		_object->_transform._position = currentPosition;

		//���� ���
		if (!_isMoving)
		{
			_walkAudio->Play();
			_isMoving = true;
		}
	}

	void BossBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// �÷��̾� ��ġ�� y���� �ޱ�.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//����ȭ.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///�÷��׸� �ɾ� ������ ���α��� ����ϱ� ���� ����.
			//ȸ���� ���� �� ���� ����.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void BossBehaviour::Dash()
	{
		// ���� ���� �ð� ���� ����
		if (_bossInfo->GetCurrentDashTime() <= _bossInfo->GetDashDuration())
		{
			_bossInfo->_status = BossStatus::DASH;

			_isRotatingToPlayer = false;
			_prohibitAttack = false;

			_collider->SetActive(false);

			float interpolation = _bossInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentDashTime(_bossInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			forwardDir.y = 0; // y�� �̵��� ���� ���� y���� 0���� ����

			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = tPosition + forwardDir * interpolation;

			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;

			if (_isMoving)
			{
				_walkAudio->Stop();
				_isMoving = false;
			}

			if (!_isRushSoundPlaying)
			{
				_rushAudio->Play();
				_isRushSoundPlaying = true;
			}
		}
		else
		{
			// ������ ������ ���¸� ����
			// ���� �ִϸ��̼��� �ٽ� �ε� �ϱ� ���� IDLE �ִϸ��̼� �߰�
			std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			_meshRenderer->SetAnimation(animId, true);
			_dashCoolTime -= _pgTime->GetDeltaTime();
			_isRotatingToPlayer = true; // �ٽ� �÷��̾ �ٶ󺸵��� ����
			_collider->SetActive(true);

			if (_dashCoolTime <= std::numeric_limits<float>::epsilon())
			{
				_hasDashed = true;
				_bossInfo->SetCurrentDashTime(0.0f); // ���� ���� �ð��� �ʱ�ȭ
				_isRushSoundPlaying = false;// ���� �ʱ�ȭ

				_dashCount++;
				_dashCoolTime = DASH_COOLTIME;
			}
		}
	}

	void BossBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(_isAttack);
		}

		_walkAudio->Stop();
	}

	void BossBehaviour::UpdateSkill()
	{
		// ��ǳ ��ų�� �̵� �� �浹 ó��
		if (_useStormBlast)
		{
			_prohibitAttack = false;
			_isRotatingToPlayer = false;

			_walkAudio->Stop();

			_bossInfo->SetCurrentWindBlastDurationTime(_bossInfo->GetCurrentWindBlastTime() + _pgTime->GetDeltaTime());

			if (_bossInfo->GetCurrentWindBlastTime() >= _bossInfo->GetStartWindBlastTime() && !_offWind)
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//�ڽ��� �ٶ󺸴� �������� �����ϱ� ������ z�໩�� ���� ����.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				_windRenderer->PlayAnim();
				_windRenderer->SetAlphaPercentage(100.f);

				if (forwardDir.z > 0)
				{
					//��ǳ �ݶ��̴� ������ ����
					for (auto& iter : _windBlastAttackCol)
					{
						iter->SetActive(true);
						iter->_object->_transform._position.z += forwardDir.z * _bossInfo->GetWindBlastSpeed() * _pgTime->GetDeltaTime();
					}
				}
				else
				{
					//��ǳ �ݶ��̴� ������ ����
					for (auto& iter : _windBlastAttackCol)
					{
						iter->SetActive(true);
						iter->_object->_transform._position.z -= forwardDir.z * _bossInfo->GetWindBlastSpeed() * _pgTime->GetDeltaTime();
					}
				}
			}
			if (_bossInfo->GetCurrentWindBlastTime() >= _bossInfo->GetWindBlastDuration())
			{
				_offWind = true;
			}
		}
	}

	void BossBehaviour::UpdatePhaseTwoSkill()
	{
		//����� ��ų�� �̵� �� �浹 ó��
		if (_useLightSkill)
		{
			_prohibitAttack = false;
			_walkAudio->Stop();
			_bossInfo->SetCurrentLightSkillTime(_bossInfo->GetCurrentLightSkillTime() + _pgTime->GetDeltaTime());

			// ����� �ݶ��̴��� ������ ��ġ�� ���������� ����
			if (_bossInfo->GetCurrentLightSkillTime() >= _bossInfo->GetStartLightSkillTime())
			{
				if (_bossInfo->GetCurrentLightSkillTime() >= _nextActivationTime)
				{
					//�ڽ��� ������ �ȴ�.
					//_collider->SetActive(false);

					if (!_isLightSkillSoundPlaying)
					{
						_laserAttack->Play();
						_isLightSkillSoundPlaying = true;
					}

					if (_currentColIndex < _lightAttackCol.size())
					{
						auto& iter = _lightAttackCol[_currentColIndex];
						auto& iter2 = _lightSkillRenderer[_currentColIndex];

						iter->SetActive(true);
						iter2->SetAlphaPercentage(100.f);

						iter2->PlayAnim();

						_isLightSkillSoundPlaying = false;

						//BattleArea�� ���� ���� ������ ����
						Pg::Math::PGFLOAT3 randomPosition = { RandomRange(-10.f, 10.f), 0, RandomRange(-10.f,10.f) };
						iter->_object->_transform._position = randomPosition;

						_currentColIndex++;
						_nextActivationTime += _activationInterval;
					}
				}
			}
			if (_bossInfo->GetCurrentLightSkillTime() >= _bossInfo->GetLightSkillDuration())
			{
				// ����� �ݶ��̴� ��Ȱ��ȭ
				for (auto& iter : _lightAttackCol)
				{
					iter->SetActive(false);
					iter->_object->_transform._position = { 0.f, -100.f, 0.f }; // ��Ȱ��ȭ ��ġ�� ����
				}
				for (auto& iter : _lightSkillRenderer)
				{
					iter->SetAlphaPercentage(0.f);
					iter->SetAnimation("bosspillar_0.pganim", false);
					iter->PauseAnim();
				}

				_bossInfo->SetCurrentLightSkillTime(0.f);
				_useLightSkill = false;
				_currentColIndex = 0;       // �ʱ�ȭ
				_nextActivationTime = 0.0f; // �ʱ�ȭ

				//���� ����
				//_collider->SetActive(true);
			}
		}
	}

	void BossBehaviour::UpdatePhaseThreeSkill()
	{
		if (_useTakeDownSkill)
		{
			if (!_takeDownFinish)
			{
				_prohibitAttack = true;
			}
			_walkAudio->Stop();
			_collider->SetActive(true);

			// Tween ����
			if (!_isRiseTween)
			{
				_upSound->Play();
				PG_WARN("TAKINGDOWN");
				_riseTween = _pgTween->CreateTween();
				_isRiseTween = true;


				_isRotatingToPlayer = false;

				// ���� ��ġ ����
				Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;

				// ��ǥ ��ġ ����
				Pg::Math::PGFLOAT3 risePosition = currentPosition;
				risePosition.y += 5.f; // ����� �Ÿ�

				// ��� Tween ����
				_riseTween->GetData(&(_object->_transform._position))
					.DoMove(risePosition, 2.f) // 2�� ���� ���
					.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART)
					.OnComplete([this]()
						{
							_goUp = true;
							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_1;

							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_2;

							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_3;
							
							_upSound->Stop();
						});
			}
		}
		if (_goUp)
		{
			_prohibitAttack = true;
			_collider->SetActive(true);
			
			//������� �ݶ��̴� Ȱ��ȭ
			for (auto& iter : _takeDownCol)
			{
				iter->SetActive(true);
			}

			// Tween ����
			if (!_isFallTween)
			{
				_downAudio->Play();
				PG_WARN("GOINGUP");
				_fallTween = _pgTween->CreateTween();
				_isFallTween = true;

				Pg::Math::PGFLOAT3 fallPosition = _playerTransform->_position;
				fallPosition.y += 4.0f;

				// �ϰ� Tween ����
				_fallTween->GetData(&(_object->_transform._position))
					.DoMove(fallPosition, 1.f) // 1�� ���� �ϰ�
					.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART)
					.OnComplete([this]()
						{
							// ������� �� �߰� ����
							_useTakeDownSkill = false;
							_isRotatingToPlayer = true;

							//������� �ݶ��̴� Ȱ��ȭ
							for (auto& iter : _takeDownCol)
							{
								iter->SetActive(false);
							}
							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_1)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;


							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_2)
							{
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;
							}
							
							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_3)
							{
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::CAST;

								//�� ������ �ݶ��̴� �ѱ�
								_collider->SetActive(true);
								_prohibitAttack = false;
								_takeDownFinish = true;
							}

							_isGenerateCol = true;
							_downAudio->Stop();
						});
			}
			if (_isGenerateCol)
			{
				_isRiseTween = false;
				_isFallTween = false;
				_goUp = false;

				if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2 ||
					_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3)
				{
					_useTakeDownSkill = true;
					_isGenerateCol = false;
				}
			}
		}
	}

	void BossBehaviour::Evade()
	{
		// ȸ�� ���� ����

		///��Ÿ�� �߰� �ʿ���(5�� ����)
		if (_bossInfo->GetCurrentEvadeTime() <= _bossInfo->GetEvadeDuration())
		{
			_bossInfo->_status = BossStatus::EVADE;

			float interpolation = _bossInfo->GetEvadeSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentEvadeTime(_bossInfo->GetCurrentEvadeTime() + _pgTime->GetDeltaTime());

			//�����̾�� ��.
			//_collider->SetActive(false);

			// ȸ�� ���� ���� (��: �÷��̾��� �ݴ� ��������)
			Pg::Math::PGFLOAT3 backwardDir = -Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			backwardDir.y = 0; // y�� �̵��� ���� ���� y���� 0���� ����

			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = tPosition + backwardDir * interpolation;

			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;
		}
		else
		{
			//_collider->SetActive(true);
			_isEvading = false;
			_bossInfo->SetCurrentEvadeTime(0.0f); // ���� ȸ�� �ð��� �ʱ�ȭ
			_isRotatingToPlayer = true; // �ٽ� �÷��̾ �ٶ󺸵��� ����
		}
	}

	void BossBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;

		//ī�޶� ��鸲
		_cameraShake->CauseShake(0.25f);
		_hitAudio->Play();

		//�ǰ� �ִϸ��̼� ���� ��.
		if (_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::IDLE ||
			_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::CHASE) return;

		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00010.pganim");

		_meshRenderer->SetAnimation(animId, false);
	}

	void BossBehaviour::Neutralize()
	{
		bool isDown = false;

		if ((_bossInfo->GetMonsterHp() <= 300.f && _monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1) ||
			(_bossInfo->GetMonsterHp() <= 200.0f && _monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_2) ||
			(_bossInfo->GetMonsterHp() <= 100.0f && _monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3))
		{
			isDown = true;
		}

		if (!isDown) return;

		if (!_isNeutralize && !_monsterHelper->_bossFlag._isDown && !_monsterHelper->_bossFlag._isDownEnd)
		{
			_monsterHelper->_bossFlag._isDownInit = true;

			if (!_isDownSoundPlaying)
			{
				_downAudio->Play();
				_isDownSoundPlaying = true;
			}

			// ����� �ݶ��̴� ��Ȱ��ȭ
			for (auto& iter : _lightAttackCol)
			{
				iter->SetActive(false);
				iter->_object->_transform._position = { 0.f, -100.f, 0.f }; // ��Ȱ��ȭ ��ġ�� ����
			}
			for (auto& iter : _lightSkillRenderer)
			{
				iter->SetAlphaPercentage(0.f);
				iter->SetAnimation("bosspillar_0.pganim", false);
				iter->PauseAnim();
			}

			//���ݵ� �� ����
			for (auto& iter : _basicAttackCol)
			{
				iter->SetActive(false);
			}
		}
		if (_monsterHelper->_bossFlag._isDown) _isNeutralize = true;
		if (_monsterHelper->_bossFlag._isDownEnd) _isNeutralizeInit = false;

		if (_isNeutralize)
		{
			//����ȭ ���� ����.
			_bossInfo->SetCurrentNeutralize(_bossInfo->GetCurrentNeutralize() + _pgTime->GetDeltaTime());

			_isRotatingToPlayer = false;
			_walkAudio->Stop();

			// �ð��� ������ ���¸� ����
			if (_isNeutralizeInit && _bossInfo->GetCurrentNeutralize() >= _bossInfo->GetEndNeutralize())
			{
				//����ȭ ����.
				_isNeutralize = false;
				_monsterHelper->_bossFlag._isDown = false;
				_monsterHelper->_bossFlag._isDownEnd = true;
				_isRotatingToPlayer = true;
				_isDownSoundPlaying = false;
				_bossInfo->SetCurrentNeutralize(0.f);
			}

			if (!_isNeutralizeInit)
			{
				_monsterHelper->_isChase = false;
				_monsterHelper->_isDistanceClose = false;
				_isNeutralizeInit = true;
			}
		}
	}

	void BossBehaviour::Dead()
	{
		if (!_isDeadInit)
		{
			_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::DEAD;

			_monsterHelper->_isPlayerDetected = false;
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isChase = false;
			_monsterHelper->_bossFlag._isDown = false;

			_monsterHelper->_isDead = true;

			_walkAudio->Stop();
			_dieAudio->Play();
			_isDeadInit = true;

			_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
		}
	}
	float BossBehaviour::RandomRange(float min, float max)
	{
		return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}

	void BossBehaviour::ResetAll()
	{
		//�÷��� ���� �ʱ�ȭ.
		_distance = 0.f;
		_isPlayerInit = false;
		_dashCount = 0;
		_isRotatingToPlayer = true;
		_rotateToPlayerTime = 0.f;

		_isNeutralizeInit = false;
		_isNeutralize = false;
		_isEvading = false;
		_hasEvaded = false;
		_evadeCooldownTime = 0.f;

		_isMoving = false;
		_isRushSoundPlaying = false;

		_isDeadInit = false;

		_useStormBlast = false;
		_offWind = false;

		_useLightSkill = false;

		_useTakeDownSkill = false;

		_goUp = false;
		_isGenerateCol = false;

		//�浹 ��ü ���� �ʱ�ȭ.
		_collider->SetActive(true);
		_meshRenderer->SetActive(true);

		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _windBlastAttackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _lightAttackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _takeDownCol)
		{
			iter->SetActive(false);
		}

		// �ִϸ��̼� ���� ���� �ʱ�ȭ
		_monsterHelper->Reset();
	}
}