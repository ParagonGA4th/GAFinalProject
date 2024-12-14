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
		//������ �� �������� ������ ��� ��.
		_enemy = _object->GetScene()->FindObjectsWithTag("TAG_Boss");
		_enemyBehaviour = _enemy.at(0)->GetComponent<Pg::DataScript::BossBehaviour>();
	}

	void EnemyAttack::Start()
	{
	}

	void EnemyAttack::Update()
	{
		//�������� ���°� �Ϲ� ������ ��
		//if (_enemyBehaviour->_bossGolStat == BossGolemStatus::BASIC_ATTACK)
		//{
		//	//���� ���� Ȱ��ȭ
		//	_collider->SetActive(true);
		//}
		////��ų ������ ��
		//else if (_enemyBehaviour->_bossGolStat == BossGolemStatus::SKILL_ATTACK)
		//{
		//
		//}
		//else if(_enemyBehaviour->_bossGolStat == BossGolemStatus::CHASE ||
		//	_enemyBehaviour->_bossGolStat == BossGolemStatus::IDLE)
		//{
		//	//���� ���� ��Ȱ��ȭ
		//	_collider->SetActive(false);
		//}
	}
}