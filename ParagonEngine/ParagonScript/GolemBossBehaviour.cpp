#include "GolemBossBehaviour.h"
#include "CameraShake.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

#include "TotalGameManager.h"
#include "BaseEnemyHandler.h"

namespace Pg::DataScript
{
	GolemBossBehaviour::GolemBossBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false), _currentAttackTime(0.f), _startAttackTime(0.7f), _endAttackTime(2.7f),
		_respawnPos(0.f, 0.f, 0.f), _areaIndex(0)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//골렘의 체력과 공격
		_golBossInfo = new GolemBossInfo(20.f, 2.f);

		///골렘의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_golBossInfo->_onDead = [this]() { Dead(); };

		_golBossInfo->_onHit = [this]() { Hit(); };
	}

	void GolemBossBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void GolemBossBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void GolemBossBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_attackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticSphereCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticSphereCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}
	}

	void GolemBossBehaviour::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		//체력과 기본 공격력을 설정해준다.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);

		//자신이 속한 곳의 AreaHandler / EnemyHandler를 받아오기.
		//적 보고 로직 등에 사용될 것.
		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}
	}

	void GolemBossBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//AudioSource 컴포넌트 들고오기
		//_miniGolemHit = _object->GetScene()->FindObjectWithName("MiniGolemHitSound");
		//_hitSound = _miniGolemHit->GetComponent<Pg::Data::AudioSource>();
		//
		//_miniGolemDie = _object->GetScene()->FindObjectWithName("MiniGolemDeadSound");
		//_dieSound = _miniGolemDie->GetComponent<Pg::Data::AudioSource>();
		//
		//_miniGolemDash = _object->GetScene()->FindObjectWithName("MiniGolemDashSound");
		//_dashSound = _miniGolemDash->GetComponent<Pg::Data::AudioSource>();
		//
		//_miniGolemAttack = _object->GetScene()->FindObjectWithName("MiniGolemAttackSound");
		//_attackSound = _miniGolemAttack->GetComponent<Pg::Data::AudioSource>();

		if (_object->GetComponent<Pg::Data::MonsterHelper>() == nullptr)
			_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();
	}

	void GolemBossBehaviour::Update()
	{
		auto plVec = _player;
		auto plTrans = plVec->_transform;

		_distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDeadDelay = false;
			_monsterHelper->_isDead = true;
		}

		// 시야 안에 들어왔을 때 쫓아가라.
		if (_distance <= _golBossInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			RotateToPlayer(_playerTransform->_position);

			if (_distance <= _golBossInfo->GetDashRange() && _isDash == false && _hasDashed == false)
			{
				_isDash = true;
				_monsterHelper->_mGolemFlag._isDash = _isDash;
				_monsterHelper->_isChase = !_isDash;
				_golBossInfo->SetCurrentDashTime(0.f);
			}

			//대쉬 true면 돌진해!!
			if (_isDash)
			{
				Dash();
			}
			else
			{
				_monsterHelper->_isChase = !_isDash;
				Chase();
			}

		}
		//시야에서 벗어나면 돌진 초기화
		else
		{
			_isDash = false;
			_hasDashed = false;
		}
		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
	}

	void GolemBossBehaviour::Idle()
	{

	}

	void GolemBossBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _golBossInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		//일정 사정거리 안에 들어오면
		if (_distance <= _golBossInfo->GetAttackRange())
		{
			//상태 변경.
			_golBossInfo->_status = GolemBossStatus::BASIC_ATTACK;

			_monsterHelper->_isChase = false;
			_monsterHelper->_isPlayerinHitSpace = true;
			_monsterHelper->_bGolemFlag._isPase_1 = true;

			if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK)
			{
				Skill(_monsterHelper->_isAnimationEnd); // 스킬 사용
			}
			if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::BASIC_ATTACK_1 ||
				_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::BASIC_ATTACK_2 ||
				_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::BASIC_ATTACK_3)
			{
				Attack(_monsterHelper->_isAnimChange);
			}
			if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::IDLE)
			{
				Attack(false);
				Skill(false);
			}
		}
		else
		{
			//상태를 Chase로 변경.
			_golBossInfo->_status = GolemBossStatus::CHASE;

			Attack(false);
			//사운드 초기화
			_isAttackSoundPlaying = false;
			_currentAttackTime = 0.f;

			// 플레이어가 시야 안에 있으면
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

			//Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			//tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
			//_object->_transform._position.x = tPosition.x;
			//_object->_transform._position.z = tPosition.z;
		}
	}

	void GolemBossBehaviour::Dash()
	{
		// 돌진 지속 시간 동안 돌진
		if (_golBossInfo->GetCurrentDashTime() < _golBossInfo->GetDashDuration())
		{
			_golBossInfo->_status = GolemBossStatus::DASH;

			if (!_isDashSoundPlaying) {
				//_dashSound->Play();
				_isDashSoundPlaying = true;
			}

			float interpolation = _golBossInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_golBossInfo->SetCurrentDashTime(_golBossInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			// 돌진 시 준비 동작 애니매이션 때문
			if (_golBossInfo->GetCurrentDashTime() >= 0.4f)
			{
				Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
				tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
				_object->_transform._position.x = tPosition.x;
				_object->_transform._position.z = tPosition.z;
			}
		}
		// 돌진이 끝나면 상태를 변경
		else
		{
			_isDash = false;
			_hasDashed = true;
			_isDashSoundPlaying = false;
			_monsterHelper->_mGolemFlag._isDash = _isDash;
			_monsterHelper->_isChase = !_isDash;
		}
	}

	void GolemBossBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;

		PG_TRACE("Hit!");

		_cameraShake->CauseShake(0.25f);
		//_hitSound->Play();

		//피격 애니메이션 들어가야 함.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00003.pganim");

		_meshRenderer->SetAnimation(animId, false);

		std::string objName = _object->GetName();
		objName = objName.substr(0, objName.rfind("_"));
		objName.append("_Crtstal");

		auto tchild = _object->_transform.GetChild(objName);
		auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_10003.pganim");
		tcMeshRenderer->SetAnimation(animId, false);
	}

	void GolemBossBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///플래그를 걸어 돌진의 여부까지 계산하기 위해 세팅.
		if (_isRotateFinish == false)
		{
			//회전이 끝날 때 까지 돌기.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}
	}

	void GolemBossBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void GolemBossBehaviour::Skill(bool _isSkill)
	{
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(_isSkill);
		}
	}

	void GolemBossBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_golBossInfo->_status = GolemBossStatus::DEAD;
		//_dieSound->Play();

		//중간에 사운드가 안꺼질 경우를 대비해 싹 다 종료.
		//_hitSound->Stop();
		//_attackSound->Stop();
		//_dashSound->Stop();

		_collider->SetActive(false);
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerDetected = false;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;
		_monsterHelper->_mGolemFlag._isDash = false;
		_isRotateFinish = true;

		//이제, Handler에게 자신이 죽었다는 것을 알려주자.
		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
	}
}