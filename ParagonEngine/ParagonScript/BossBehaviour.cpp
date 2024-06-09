#include "BossBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
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
		_collider->FreezeLinearY(true);
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

		//골렘의 정보 설정
		_bossGolInfo.SetHp(10.f);
		_bossGolInfo.SetAttackRange(3.f);
	}

	void BossBehaviour::Update()
	{
		RotateToPlayer(_playerTransform->_position);
		Chase();
	}


	void BossBehaviour::OnCollisionEnter(Pg::Data::PhysicsCollision** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::PhysicsCollision* tCol = _colArr[i];

			//충돌해오는 Actor들을 검사.
			Pg::Data::Collider* arrowCol = Pg::Data::PhysicsCollision::GetActualOtherActor(tCol, this->_object);

			//Physics Layer로 검사한다. 화살에 맞았을 때.
			if (arrowCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES)
			{
				//화살에 맞았을 때 피격행동 및 상태 구현.

				//체력이 1씩 닳는다.
				_bossGolInfo._hp -= 1.f;

				//체력이 다 까이면
				if (_bossGolInfo._hp <= 0.f)
				{
					//죽는 애니메이션 먼저 호출 필요함.
					//애니메이션 전체 재생 후 다음으로 넘어가야 함.

					//죽는다.
					Dead();
				}
			}
		}
	}

	void BossBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _bossGolInfo._moveSpeed * _pgTime->GetDeltaTime();

		//auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
		//auto plTrans = plVec.at(0)->_transform;

		auto plVec = _player;
		auto plTrans = plVec->_transform;

		float distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		//일정 사정거리 안에 들어오면
		if (distance <= _bossGolInfo._attackRange)
		{

			//상태 변경.
			_bossGolStat = BossGolemStatus::BASIC_ATTACK;

			//공격으로 전환하기.
			Attack();

			// 공격 애니메이션 출력.
			_monsterHelper->_isPlayerinHitSpace = true;
			_monsterHelper->_isDistanceClose = false;
		}
		else
		{
			//상태를 Chase로 변경.
			_bossGolStat = BossGolemStatus::CHASE;

			// 플레이어가 시야 안에 있으면
			_monsterHelper->_isPlayerDetected = true;
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isDistanceClose = true;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, plTrans._position, interpolation);
			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;
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
		if (!_isRotateFinish)
		{
			//회전이 끝날 때 까지 돌기.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}
	}

	void BossBehaviour::Attack()
	{

	}

	void BossBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_bossGolStat = BossGolemStatus::DEAD;

		//다 꺼짐.
		_collider->SetActive(false);
		_meshRenderer->SetActive(false);
		_object->SetActive(false);
	}
}