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

		//골렘의 체력과 공격
		_bossInfo = new BossInfo(MAX_BOSS_HEALTH, BOSS_ATTACK_POWER);
		//_bossInfo = new BossInfo(2.f, 4.f);

		///보스의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
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

		//플레이어 지정
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		_bossWalkSound = _object->GetScene()->FindObjectWithName("BossWalkSound");
		_walkAudio = _bossWalkSound->GetComponent<Pg::Data::AudioSource>();

		_bossRushSound = _object->GetScene()->FindObjectWithName("BossRushSound");
		_rushAudio = _bossRushSound->GetComponent<Pg::Data::AudioSource>();

		_bossDieSound = _object->GetScene()->FindObjectWithName("BossDieSound");
		_dieAudio = _bossDieSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _downSound = _object->GetScene()->FindObjectWithName("BossDownSound");
		_downAudio = _downSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _basicAttackSound1 = _object->GetScene()->FindObjectWithName("BossAttackSound1");
		_basicAttackAudio1 = _basicAttackSound1->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _basicAttackSound2 = _object->GetScene()->FindObjectWithName("BossAttackSound2");
		_basicAttackAudio2 = _basicAttackSound2->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _hit = _object->GetScene()->FindObjectWithName("BossHitSound");
		_hitAudio = _hit->GetComponent<Pg::Data::AudioSource>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_meshRenderer->SetRendererOffset(_rendererOffset);
		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		_wind = _object->GetScene()->FindObjectWithName("BossWindBlastEffect");
		_windRenderer = _wind->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
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

		//clear 필요함.
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
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
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

		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;

			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			//게임을 이겼다고 (==Boss 죽였다고 Event Post) 알리기!
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
					Attack(_monsterHelper->_isAnimChange);

					//_isRotatingToPlayer = true;
					//_useTakeDownSkill = true;
				}
				if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::BASIC_ATTACK_3)
				{
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

		//돌풍 스킬
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
			}
		}
		//빛기둥 스킬
		UpdatePhaseTwoSkill();

		//내려찍기 스킬
		UpdatePhaseThreeSkill();

		_bossInfo->UpdateBaseMonsterLogic(_object);
	}

	void BossBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _bossInfo->GetMoveSpeed() * _bossInfo->GetMonsterSpeedRatio() * _pgTime->GetDeltaTime();

		//상태를 Chase로 변경.

		//애니메이션
		_monsterHelper->_isPlayerDetected = true;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = true;

		//사정거리 밖이면 플레이어로 계속 다가가기.
		///보간하면서 이동할 시 마지막에 느려지는 현상을 발생하기 위해 제거.
		Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;
		Pg::Math::PGFLOAT3 targetPosition = _playerTransform->_position;

		// 목표 지점까지의 방향 벡터 계산
		Pg::Math::PGFLOAT3 direction = targetPosition - currentPosition;
		direction.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

		// 방향 벡터를 정규화
		Pg::Math::PGFLOAT3 directionNorm = Pg::Math::PGFloat3Normalize(direction);

		// 일정한 속도로 이동
		Pg::Math::PGFLOAT3 movement = directionNorm * interpolation;

		currentPosition.x += movement.x;
		currentPosition.z += movement.z;

		_object->_transform._position = currentPosition;

		//사운드 재생
		if (!_isMoving)
		{
			_walkAudio->Play();
			_isMoving = true;
		}
	}

	void BossBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///플래그를 걸어 돌진의 여부까지 계산하기 위해 세팅.
			//회전이 끝날 때 까지 돌기.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void BossBehaviour::Dash()
	{
		// 돌진 지속 시간 동안 돌진
		if (_bossInfo->GetCurrentDashTime() <= _bossInfo->GetDashDuration())
		{
			_bossInfo->_status = BossStatus::DASH;

			_isRotatingToPlayer = false;

			float interpolation = _bossInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentDashTime(_bossInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			forwardDir.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

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
			// 돌진이 끝나면 상태를 변경
			// 돌진 애니매이션을 다시 로드 하기 위해 IDLE 애니매이션 추가
			std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			_meshRenderer->SetAnimation(animId, true);
			_dashCoolTime -= _pgTime->GetDeltaTime();
			_isRotatingToPlayer = true; // 다시 플레이어를 바라보도록 설정

			if (_dashCoolTime <= std::numeric_limits<float>::epsilon())
			{
				_hasDashed = true;
				_bossInfo->SetCurrentDashTime(0.0f); // 현재 돌진 시간을 초기화
				_isRushSoundPlaying = false;// 사운드 초기화

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
		// 돌풍 스킬의 이동 및 충돌 처리
		if (_useStormBlast)
		{
			_isRotatingToPlayer = false;

			_walkAudio->Stop();

			_bossInfo->SetCurrentWindBlastDurationTime(_bossInfo->GetCurrentWindBlastTime() + _pgTime->GetDeltaTime());

			if (_bossInfo->GetCurrentWindBlastTime() >= _bossInfo->GetStartWindBlastTime() && !_offWind)
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//자신이 바라보는 방향으로 쏴야하기 때문에 z축빼고 전부 고정.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				_windRenderer->PlayAnim();
				_windRenderer->SetAlphaPercentage(100.f);

				if (forwardDir.z > 0)
				{
					//돌풍 콜라이더 앞으로 전진
					for (auto& iter : _windBlastAttackCol)
					{
						iter->SetActive(true);
						iter->_object->_transform._position.z += forwardDir.z * _bossInfo->GetWindBlastSpeed() * _pgTime->GetDeltaTime();
					}
				}
				else
				{
					//돌풍 콜라이더 앞으로 전진
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
		//빛기둥 스킬의 이동 및 충돌 처리
		if (_useLightSkill)
		{
			_walkAudio->Stop();
			_bossInfo->SetCurrentLightSkillTime(_bossInfo->GetCurrentLightSkillTime() + _pgTime->GetDeltaTime());

			// 빛기둥 콜라이더를 임의의 위치에 순차적으로 생성
			if (_bossInfo->GetCurrentLightSkillTime() >= _bossInfo->GetStartLightSkillTime())
			{
				if (_bossInfo->GetCurrentLightSkillTime() >= _nextActivationTime)
				{
					//자신은 무적이 된다.
					//_collider->SetActive(false);

					if (_currentColIndex < _lightAttackCol.size())
					{
						auto& iter = _lightAttackCol[_currentColIndex];
						auto& iter2 = _lightSkillRenderer[_currentColIndex];

						iter->SetActive(true);
						iter2->SetAlphaPercentage(100.f);

						iter2->PlayAnim();

						//BattleArea의 값에 따라 수정할 예정
						Pg::Math::PGFLOAT3 randomPosition = { RandomRange(-10.f, 10.f), 0, RandomRange(-10.f,10.f) };
						iter->_object->_transform._position = randomPosition;

						_currentColIndex++;
						_nextActivationTime += _activationInterval;
					}
				}
			}
			if (_bossInfo->GetCurrentLightSkillTime() >= _bossInfo->GetLightSkillDuration())
			{
				// 빛기둥 콜라이더 비활성화
				for (auto& iter : _lightAttackCol)
				{
					iter->SetActive(false);
					iter->_object->_transform._position = { 0.f, -100.f, 0.f }; // 비활성화 위치로 설정
				}
				for (auto& iter : _lightSkillRenderer)
				{
					iter->SetAlphaPercentage(0.f);
					iter->SetAnimation("bosspillar_0.pganim", false);
					iter->PauseAnim();
				}

				_bossInfo->SetCurrentLightSkillTime(0.f);
				_useLightSkill = false;
				_currentColIndex = 0;       // 초기화
				_nextActivationTime = 0.0f; // 초기화

				//무적 해제
				//_collider->SetActive(true);
			}
		}
	}

	void BossBehaviour::UpdatePhaseThreeSkill()
	{
		if (_useTakeDownSkill)
		{
			_walkAudio->Stop();

			// Tween 생성
			if (!_isRiseTween)
			{
				PG_WARN("TAKINGDOWN");
				_riseTween = _pgTween->CreateTween();
				_isRiseTween = true;

				_collider->SetActive(false);

				_isRotatingToPlayer = false;

				// 현재 위치 저장
				Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;

				// 목표 위치 설정
				Pg::Math::PGFLOAT3 risePosition = currentPosition;
				risePosition.y += 5.f; // 상승할 거리

				// 상승 Tween 설정
				_riseTween->GetData(&(_object->_transform._position))
					.DoMove(risePosition, 2.f) // 2초 동안 상승
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
						});
			}
		}
		if (_goUp)
		{
			//내려찍기 콜라이더 활성화
			for (auto& iter : _takeDownCol)
			{
				iter->SetActive(true);
			}

			// Tween 생성
			if (!_isFallTween)
			{
				PG_WARN("GOINGUP");
				_fallTween = _pgTween->CreateTween();
				_isFallTween = true;

				Pg::Math::PGFLOAT3 fallPosition = _playerTransform->_position;
				fallPosition.y += 4.0f;

				// 하강 Tween 설정
				_fallTween->GetData(&(_object->_transform._position))
					.DoMove(fallPosition, 1.f) // 1초 동안 하강
					.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART)
					.OnComplete([this]()
						{
							// 내려찍기 후 추가 동작
							_useTakeDownSkill = false;
							_isRotatingToPlayer = true;

							//내려찍기 콜라이더 활성화
							for (auto& iter : _takeDownCol)
							{
								iter->SetActive(false);
							}
							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_1)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;


							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_2)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;
							
							if (_monsterHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FLY_ATTACK_3)
								_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::DASH;

							_isGenerateCol = true;
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
		// 회피 로직 구현

		///쿨타임 추가 필요함(5초 정도)
		if (_bossInfo->GetCurrentEvadeTime() <= _bossInfo->GetEvadeDuration())
		{
			_bossInfo->_status = BossStatus::EVADE;

			float interpolation = _bossInfo->GetEvadeSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentEvadeTime(_bossInfo->GetCurrentEvadeTime() + _pgTime->GetDeltaTime());

			//무적이어야 함.
			//_collider->SetActive(false);

			// 회피 방향 설정 (예: 플레이어의 반대 방향으로)
			Pg::Math::PGFLOAT3 backwardDir = -Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			backwardDir.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = tPosition + backwardDir * interpolation;

			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;
		}
		else
		{
			//_collider->SetActive(true);
			_isEvading = false;
			_bossInfo->SetCurrentEvadeTime(0.0f); // 현재 회피 시간을 초기화
			_isRotatingToPlayer = true; // 다시 플레이어를 바라보도록 설정
		}
	}

	void BossBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;

		//카메라 흔들림
		_cameraShake->CauseShake(0.25f);
		_hitAudio->Play();

		//피격 애니메이션 들어가야 함.
		if (_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::IDLE ||
			_monsterHelper->_bossFlag._bossState != Pg::Data::BossState::CHASE) return;

		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00010.pganim");

		_meshRenderer->SetAnimation(animId, false);
	}

	void BossBehaviour::Neutralize()
	{
		bool isDown = false;

		if ((_bossInfo->GetMonsterHp() <= 600.f && _monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1) ||
			_bossInfo->GetMonsterHp() <= 400.0f && _monsterHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_2)
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
		}
		if (_monsterHelper->_bossFlag._isDown) _isNeutralize = true;
		if (_monsterHelper->_bossFlag._isDownEnd) _isNeutralizeInit = false;

		if (_isNeutralize)
		{
			//무력화 상태 시작.
			_bossInfo->SetCurrentNeutralize(_bossInfo->GetCurrentNeutralize() + _pgTime->GetDeltaTime());

			_isRotatingToPlayer = false;
			_walkAudio->Stop();

			// 시간이 끝나면 상태를 변경
			if (_isNeutralizeInit && _bossInfo->GetCurrentNeutralize() >= _bossInfo->GetEndNeutralize())
			{
				//무력화 해제.
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
			//_monsterHelper->_bossFlag._bossState = Pg::Data::BossState::DEAD;
			//_monsterHelper->_bossFlag._bossPase = Pg::Data::BossPase::PASE_1;

			//_monsterHelper->_isPlayerDetected = false;
			//_monsterHelper->_isPlayerinHitSpace = false;
			//_monsterHelper->_isChase = false;
			//_monsterHelper->_bossFlag._isDash = false;
			//_monsterHelper->_bossFlag._isDown = false;

			_monsterHelper->Reset();
			_monsterHelper->_isDead = true;
			_isPlayerInit = false;

			_dashCount = 0;

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
		//플래그 전부 초기화.
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

		//충돌 객체 전부 초기화.
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

		// 애니매이션 관련 전부 초기화
		//_monsterHelper->Reset();
	}
}