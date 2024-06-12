#include "MiniGolemBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

namespace Pg::DataScript
{
	MiniGolemBehaviour::MiniGolemBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//골렘의 체력과 공격
		_miniGolInfo = new MiniGolemInfo(5.f, 1.f);

		///골렘의 사망행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_miniGolInfo->_onDead = [this]() { Dead(); };
	}

	void MiniGolemBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearY(true);
	}

	void MiniGolemBehaviour::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		//체력과 기본 공격력을 설정해준다.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);
	}

	void MiniGolemBehaviour::Start()
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

	void MiniGolemBehaviour::Update()
	{
		auto plVec = _player;
		auto plTrans = plVec->_transform;

		_distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		// 시야 안에 들어왔을 때 쫓아가라.
		if (_distance <= _miniGolInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			RotateToPlayer(_playerTransform->_position);

			if (_distance <= _miniGolInfo->GetDashRange() && _isDash == false && _hasDashed == false)
			{
				_isDash = true;
				_monsterHelper->_isDash = _isDash;
				_monsterHelper->_isChase = !_isDash;
				_miniGolInfo->SetCurrentDashTime(0.f);
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


		if (_monsterHelper->_isAnimationEnd && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);
			_monsterHelper->_isAnimationEnd = false;
		}

		PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
	}

	void MiniGolemBehaviour::Idle()
	{

	}

	void MiniGolemBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _miniGolInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		//일정 사정거리 안에 들어오면
		if (_distance <= _miniGolInfo->GetAttackRange())
		{

			//상태 변경.
			_miniGolInfo->_status = MiniGolemStatus::BASIC_ATTACK;

			//공격
			Attack(true);

			// 공격 애니메이션 출력.
			_monsterHelper->_isPlayerinHitSpace = true;
		}
		else
		{
			//상태를 Chase로 변경.
			_miniGolInfo->_status = MiniGolemStatus::CHASE;

			Attack(false);

			// 플레이어가 시야 안에 있으면
			_monsterHelper->_isPlayerinHitSpace = false;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;
		}
	}

	void MiniGolemBehaviour::Dash()
	{
		// 돌진 지속 시간 동안 돌진
		if (_miniGolInfo->GetCurrentDashTime() < _miniGolInfo->GetDashDuration())
		{
			_miniGolInfo->_status = MiniGolemStatus::DASH;

			float interpolation = _miniGolInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_miniGolInfo->SetCurrentDashTime(_miniGolInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			// 돌진 시 준비 동작 애니매이션 때문
			if (_miniGolInfo->GetCurrentDashTime() >= 0.4f)
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
			_monsterHelper->_isDash = _isDash;
			_monsterHelper->_isChase = !_isDash;
		}
	}

	void MiniGolemBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///플래그를 걸어 돌진의 여부까지 계산하기 위해 세팅.
		if (!_isRotateFinish)
		{
			//회전이 끝날 때 까지 돌기.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}
	}

	void MiniGolemBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void MiniGolemBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_miniGolInfo->_status = MiniGolemStatus::DEAD;
		_monsterHelper->_isDead = true;
	}

	BaseMonsterInfo* MiniGolemBehaviour::ReturnBaseMonsterInfo()
	{
		return _miniGolInfo;
	}
}