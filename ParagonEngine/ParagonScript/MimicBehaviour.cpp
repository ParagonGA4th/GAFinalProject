#include "MimicBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

namespace Pg::DataScript
{
	MimicBehaviour::MimicBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false), _currentAttackTime(0.f), _startAttackTime(1.f), _endAttackTime(2.7f),
		_respawnPos(0.f, 0.f, 0.f)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//미믹의 체력과 공격
		_mimicInfo = new MimicInfo(10.f, 1.f);

		///미믹의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_mimicInfo->_onDead = [this]() { Dead(); };

		_mimicInfo->_onHit = [this]() { Hit(); };
	}

	void MimicBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearY(true);
	}

	void MimicBehaviour::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		//체력과 기본 공격력을 설정해준다.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);
	}

	void MimicBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();

			if (staticCol != nullptr)
			{
				_attackCol.push_back(staticCol);
				staticCol->SetActive(false);
			}
		}
	}

	void MimicBehaviour::Update()
	{
		auto plVec = _player;
		auto plTrans = plVec->_transform;

		_distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		// 시야 안에 들어왔을 때 쫓아가라.
		if (_distance <= _mimicInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			RotateToPlayer(_playerTransform->_position);

			//대쉬 true면 돌진해!!
			if (_isDash)
			{
				
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


		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDeadDelay = false;
		}

		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
	}

	void MimicBehaviour::Idle()
	{

	}

	void MimicBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _mimicInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		//일정 사정거리 안에 들어오면
		if (_distance <= _mimicInfo->GetAttackRange())
		{
			//상태 변경.
			_mimicInfo->_status = MimicStatus::BASIC_ATTACK;

			//애니메이션 딜레이를 위한 델타타임 체크.
			_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();

			//공격
			if (_currentAttackTime >= _startAttackTime)
			{
				Attack(true);
			}
			if (_currentAttackTime >= _startAttackTime && _currentAttackTime >= _endAttackTime)
			{
				Attack(false);

				_currentAttackTime = 0.f;
			}

			// 공격 애니메이션 출력.
			_monsterHelper->_isPlayerinHitSpace = true;
		}
		else
		{
			//상태를 Chase로 변경.
			_mimicInfo->_status = MimicStatus::CHASE;

			Attack(false);
			_currentAttackTime = 0.f;

			// 플레이어가 시야 안에 있으면
			_monsterHelper->_isPlayerinHitSpace = false;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;
		}
	}

	void MimicBehaviour::Hit()
	{
		PG_TRACE("Hit!");

		//피격 애니메이션 들어가야 함.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00003.pganim");

		_meshRenderer->SetAnimation(animId, false);
	}

	void MimicBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		//회전이 끝날 때 까지 돌기.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void MimicBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void MimicBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_mimicInfo->_status = MimicStatus::DEAD;
		_monsterHelper->_isDead = true;
	}

	BaseMonsterInfo* MimicBehaviour::ReturnBaseMonsterInfo()
	{
		return _mimicInfo;
	}
}