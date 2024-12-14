#include "EnemyAttack.h"
#include "BossBehaviour.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonAPI/PgScene.h"
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	EnemyAttack::EnemyAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
	}

	void EnemyAttack::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		_collider->SetActive(false);
	}

	void EnemyAttack::Awake()
	{
		//공격을 할 보스골렘의 정보를 들고 옴.
		_enemy = _object->GetScene()->FindObjectsWithTag("TAG_Boss");
		_enemyBehaviour = _enemy.at(0)->GetComponent<Pg::DataScript::BossBehaviour>();
	}

	void EnemyAttack::Start()
	{
	}

	void EnemyAttack::Update()
	{
		//보스골렘의 상태가 일반 공격일 때
		//if (_enemyBehaviour->_bossGolStat == BossGolemStatus::BASIC_ATTACK)
		//{
		//	//공격 범위 활성화
		//	_collider->SetActive(true);
		//}
		////스킬 공격일 때
		//else if (_enemyBehaviour->_bossGolStat == BossGolemStatus::SKILL_ATTACK)
		//{
		//
		//}
		//else if(_enemyBehaviour->_bossGolStat == BossGolemStatus::CHASE ||
		//	_enemyBehaviour->_bossGolStat == BossGolemStatus::IDLE)
		//{
		//	//공격 범위 비활성화
		//	_collider->SetActive(false);
		//}
	}
}