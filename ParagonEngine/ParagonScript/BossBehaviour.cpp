#include "BossBehaviour.h"
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
		_bossInfo = new BossInfo(20.f, 2.f);

		///보스의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_bossInfo->_onDead = [this]() { Dead(); };

		_bossInfo->_onHit = [this]() { Hit(); };
	}

	void BossBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
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

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

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

	void BossBehaviour::Update()
	{
		///보스의 행동패턴 들어가야함.
		RotateToPlayer(_playerTransform->_position);
		Chase();
		neutralize();
	}

	void BossBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _bossInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		auto plVec = _player;
		auto plTrans = plVec->_transform;

		float distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));
		//상태를 Chase로 변경.

		//애니메이션
		_monsterHelper->_isPlayerDetected = true;
		_monsterHelper->_isPlayerinHitSpace = false;

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
		if (_isRotateFinish && _bossInfo->GetCurrentDashTime() < _bossInfo->GetDashDuration())
		{
			_bossInfo->_status = BossStatus::DASH;

			float interpolation = _bossInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_bossInfo->SetCurrentDashTime(_bossInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			// 돌진 시 준비 동작 애니매이션 때문
			if (_bossInfo->GetCurrentDashTime() >= 0.4f)
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
				forwardDir.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

				Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
				tPosition = tPosition + forwardDir * interpolation;

				_object->_transform._position.x = tPosition.x;
				_object->_transform._position.z = tPosition.z;
			}
		}
		// 돌진이 끝나면 상태를 변경
		else if (_bossInfo->GetCurrentDashTime() >= _bossInfo->GetDashDuration())
		{
			_isDash = false;
			_hasDashed = true;
			_monsterHelper->_isDash = _isDash;
			_monsterHelper->_isChase = !_isDash;
		}
	}

	void BossBehaviour::Attack()
	{

	}

	void BossBehaviour::Avoid()
	{

	}

	void BossBehaviour::Hit()
	{

	}

	void BossBehaviour::neutralize()
	{
		//체력이 반 이상 떨어지면
		if (_bossInfo->GetMonsterHp() <= 10.f)
		{
			//무력화 상태 시작.
			_bossInfo->SetCurrentNeutralize(_bossInfo->GetCurrentNeutralize() + _pgTime->GetDeltaTime());

			_isNeutralize = true;
			_isChasing = false;

			// 시간이 끝나면 상태를 변경
			if (_isNeutralize && _bossInfo->GetCurrentNeutralize() >= _bossInfo->GetEndNeutralize())
			{
				//무력화 해제.
				_isNeutralize = false;
				_isChasing = true;
				_bossInfo->SetCurrentNeutralize(0.f);
			}
		}

	}

	void BossBehaviour::Dead()
	{
		//다 꺼짐.
		_collider->SetActive(false);
		_meshRenderer->SetActive(false);
		_object->SetActive(false);

		///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
		_object->_transform._position = { 0, -1000, 0 };
	}

	BaseMonsterInfo* BossBehaviour::ReturnBaseMonsterInfo()
	{
		return _bossInfo;
	}

}