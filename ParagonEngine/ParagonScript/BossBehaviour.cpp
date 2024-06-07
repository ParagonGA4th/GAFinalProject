#include "BossBehaviour.h"
#include "EnemyInfo.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
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

	void BossBehaviour::Awake()
	{

	}

	void BossBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();
	}

	void BossBehaviour::Update()
	{
		RotateToPlayer(_playerTransform->_position);
		Chase();
	}

	void BossBehaviour::Chase()
	{
		float interpolation = 0.2f * _pgTime->GetDeltaTime();

		//auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
		//auto plTrans = plVec.at(0)->_transform;

		auto plVec = _player;
		auto plTrans = plVec->_transform;

		float distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		//일정 사정거리 안에 들어오면
		if (distance <= 5.f)
		{
			//공격으로 전환하기.

			//상태 변경.
			_bossGolStat = BossGolemStatus::BASIC_ATTACK;

			// 플레이어가 공격 범위 안에 있으면 
			//_monsterHelper->_isPlayerinHitSpace = true;
			//_monsterHelper->_isDistanceClose = true;
		}
		else
		{
			// 플레이어가 시야 안에 있으면
			//_monsterHelper->_isPlayerDetected = true;
			//_monsterHelper->_isPlayerinHitSpace = false;
			//_monsterHelper->_isDistanceClose = false;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, plTrans._position, interpolation);
			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;

			//상태를 Chase로 변경.
			_bossGolStat = BossGolemStatus::CHASE;
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

	}
}