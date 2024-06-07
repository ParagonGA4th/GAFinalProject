#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class MonsterHelper;
}

namespace Pg::API
{	
	class PgScene;

	namespace Time
	{
		class PgTime;
	}
}

namespace Pg::DataScript
{
	class BossBehaviour : ScriptInterface<BossBehaviour>
	{	
		DEFINE_PARAGON_SCRIPT(BossBehaviour);
	public:
		BossBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//플레이어를 쫓는 함수
		void Chase();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//플레이어를 공격.
		void Attack();

		//피격 시 죽음.
		void Dead();

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		BossGolemInfo _bossGolInfo;
		BossGolemStatus _bossGolStat;

		bool _isRotateFinish;
	};
}

