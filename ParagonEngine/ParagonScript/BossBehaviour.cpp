#include "BossBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	BossBehaviour::BossBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//골렘의 체력과 공격
		_bossInfo = new BossInfo(40.f, 4.f);

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

	void BossBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_BOSS);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childName = iter->_object->GetName();

			if (childName == "BossBasicAttackRange")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childName == "TrentSkillAttackRange")
			{

			}
		}
	}

	void BossBehaviour::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
	}

	void BossBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		_bossWalkSound = _pgScene->GetCurrentScene()->FindObjectWithName("BossWalkSound");
		_walkAudio = _bossWalkSound->GetComponent<Pg::Data::AudioSource>();

		_bossRushSound = _pgScene->GetCurrentScene()->FindObjectWithName("BossRushSound");
		_rushAudio = _bossRushSound->GetComponent<Pg::Data::AudioSource>();

		_bossDieSound = _pgScene->GetCurrentScene()->FindObjectWithName("BossDieSound");
		_dieAudio = _bossDieSound->GetComponent<Pg::Data::AudioSource>();

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();
	}

	void BossBehaviour::Update()
	{
		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));

		///회피와 돌진을 테스트하기 위한 임의의 로직.
		///애니메이션을 통한 행동 패턴에 맞게 들어갈 예정.
	// 보스가 플레이어를 바라보고 있는 시간 추적
		if (_distance <= _bossInfo->GetSightRange()) { _isPlayerInit = true; _monsterHelper->_isPlayerDetected = true; }
		if (!_isPlayerInit) return;

		Neutralize();
		if (_isNeutralize) return;

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;
		}

		if (_isRotatingToPlayer)
		{
			RotateToPlayer(_playerTransform->_position);
			_rotateToPlayerTime += _pgTime->GetDeltaTime();

			if (_dashCount <= 2)
			{
				_monsterHelper->_isDash = true;
				_isDash = true;
				Dash();
			}
			else
			{
				_isDash = false;
				_monsterHelper->_isDash = false;
			}

			if (!_isDash)
			{
				if (_isChasing)
				{
					Chase();
				}
			}
			// 3초 동안 바라본 후 돌진 시작
			//if (_rotateToPlayerTime >= 3.0f)
			//{
			//	_isRotatingToPlayer = false;
			//	_rotateToPlayerTime = 0.0f; // 타이머 초기화

			//	if (!_isEvading)
			//	{
			//		_isDash = true;
			//		_bossInfo->SetCurrentDashTime(0.0f); // 돌진 시간을 초기화하여 돌진 시작
			//	}
			//	else
			//	{
			//		_hasEvaded = true;
			//		_bossInfo->SetCurrentEvadeTime(0.0f); // 회피 시간을 초기화하여 회피 시작
			//	}
			//}
			if (!_isDash)
			{
				if (_distance <= _bossInfo->GetAttackRange())
				{
					_isChasing = false;
					_monsterHelper->_isPlayerinHitSpace = true;
					_monsterHelper->_isPase_1 = true;

					if (_monsterHelper->_bossState == Pg::Data::BossState::BASIC_ATTACK_1 ||
						_monsterHelper->_bossState == Pg::Data::BossState::BASIC_ATTACK_2 /*||
						_monsterHelper->_bossState == Pg::Data::BossState::BASIC_ATTACK_3*/)
					{
						Attack(_monsterHelper->_isAnimChange);
					}
					if (_monsterHelper->_bossState == Pg::Data::BossState::IDLE)
					{
						Attack(false);
					}
				}
				else
				{
					_isChasing = true;
				}
			}
		}
		//else
		//{
		//	if (_isDash)
		//	{
		//		Dash();
		//	}
		//	else if (_hasEvaded)
		//	{
		//		Evade();
		//	}
		//}
	}

	void BossBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _bossInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

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
		if (_isChasing == true)
		{
			//회전이 끝날 때 까지 돌기.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}
	}

	void BossBehaviour::Dash()
	{
		// 돌진 지속 시간 동안 돌진
		if (_monsterHelper->_bossState == Pg::Data::BossState::DASH)
		{
			_bossInfo->_status = BossStatus::DASH;

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
		// 돌진이 끝나면 상태를 변경
		else
		{
			// 돌진 애니매이션을 다시 로드 하기 위해 IDLE 애니매이션 추가
			std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			_meshRenderer->SetAnimation(animId, false);

			_hasDashed = true;
			_bossInfo->SetCurrentDashTime(0.0f); // 현재 돌진 시간을 초기화
			_isRotatingToPlayer = true; // 다시 플레이어를 바라보도록 설정
			_isRushSoundPlaying = false;// 사운드 초기화
			_rotateToPlayerTime = 0.f;
			_dashCount++;
		}
	}

	void BossBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void BossBehaviour::Skill(bool _isSkill)
	{
		//for (auto& iter : _skillAttackCol)
		//{
		//	iter->SetActive(_isSkill);
		//}
	}

	void BossBehaviour::Evade()
	{
		// 회피 로직 구현
		if (_monsterHelper->_bossState == Pg::Data::BossState::EVASION)
		{
			_bossInfo->_status = BossStatus::EVADE;

			float interpolation = _bossInfo->GetEvadeSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentEvadeTime(_bossInfo->GetCurrentEvadeTime() + _pgTime->GetDeltaTime());

			//무적이어야 함.
			_collider->SetActive(false);

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
			_collider->SetActive(true);
			_isEvading = false;
			_bossInfo->SetCurrentEvadeTime(0.0f); // 현재 회피 시간을 초기화
			_isRotatingToPlayer = true; // 다시 플레이어를 바라보도록 설정
		}

		//if (_bossInfo->GetCurrentEvadeTime() < _bossInfo->GetEvadeDuration())
		//{
		//	_bossInfo->_status = BossStatus::EVADE;

		//	float interpolation = _bossInfo->GetEvadeSpeed() * _pgTime->GetDeltaTime();
		//	_bossInfo->SetCurrentEvadeTime(_bossInfo->GetCurrentEvadeTime() + _pgTime->GetDeltaTime());

		//	// 회피 방향 설정 (예: 플레이어의 반대 방향으로)
		//	Pg::Math::PGFLOAT3 backwardDir = -Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
		//	backwardDir.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

		//	Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
		//	tPosition = tPosition + backwardDir * interpolation;

		//	_object->_transform._position.x = tPosition.x;
		//	_object->_transform._position.z = tPosition.z;
		//}
		//else if (_bossInfo->GetCurrentEvadeTime() >= _bossInfo->GetEvadeDuration())
		//{
		//	_isEvading = false;
		//	_bossInfo->SetCurrentEvadeTime(0.0f); // 현재 회피 시간을 초기화
		//	_isRotatingToPlayer = true; // 다시 플레이어를 바라보도록 설정
		//}
	}

	void BossBehaviour::Hit()
	{
		//피격 애니메이션 들어가야 함.
		if (_isChasing)
		{
			std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
			animId.append("_00010.pganim");

			_meshRenderer->SetAnimation(animId, false);
		}
	}

	void BossBehaviour::Neutralize()
	{
		//체력이 반 이상 떨어지면
		if (_bossInfo->GetMonsterHp() <= 10.f)
		{
			//무력화 상태 시작.
			_bossInfo->SetCurrentNeutralize(_bossInfo->GetCurrentNeutralize() + _pgTime->GetDeltaTime());

			// 시간이 끝나면 상태를 변경
			if (_isNeutralizeInit && _bossInfo->GetCurrentNeutralize() >= _bossInfo->GetEndNeutralize())
			{
				//무력화 해제.
				_isNeutralize = false;
				_monsterHelper->_isDown = false;
				_bossInfo->SetCurrentNeutralize(0.f);
				return;
			}

			if (!_isNeutralizeInit)
			{
				_monsterHelper->_isDown = true;
				_monsterHelper->_isChase = false;
				_monsterHelper->_isDash = false;
				_monsterHelper->_isDistanceClose = false;
			}

			_isNeutralize = true;
			_isNeutralizeInit = true;
		}

	}

	void BossBehaviour::Dead()
	{
		_monsterHelper->_isDead = true;
		_dieAudio->Play();
	}
}