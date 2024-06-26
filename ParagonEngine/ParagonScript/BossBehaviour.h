#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "BossInfo.h"
#include "BaseMonster.h"

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class CapsuleCollider;
	class StaticBoxCollider;
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
	class BossBehaviour : public ScriptInterface<BossBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(BossBehaviour);
	public:
		BossBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override;

		//플레이어를 쫓는 함수
		void Chase();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//특정 방향을 향해 돌진.
		//미니골렘과는 달리 플레이어를 향하는 것이 아니라 자신이 보는 방향을 향해 돌진한다.
		void Dash();

		//플레이어를 공격.
		void Attack(bool _isAttack);

		//보스의 스킬공격
		void Skill(bool _isSkill);

		//회피하는 로직.
		void Evade();

		//보스의 무력화 상태
		void Neutralize();

		//피격 시 행동.
		void Hit();

		//피격 시 죽음.
		void Dead();

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::CapsuleCollider* _collider;

		std::vector<Pg::Data::StaticBoxCollider*> _basicAttackCol;

	public:
		//골렘보스의 상태와 수치에 대한 정보.
		BossInfo* _bossInfo;

		float _distance;				//보스와 플레이어와의 거리
		bool _isPlayerInit{ false };

		bool _isRotateFinish;

		//대쉬 관련 변수.
		bool _isDash{ false };			//돌진 여부
		bool _hasDashed{ false };		//돌진했는지 여부
		int _dashCount{ 0 };			//몇 번 돌진 했는지

		bool _isNeutralizeInit{ false };	//무력화 여부
		bool _isNeutralize{ false };		//무력화 상태 여부

		bool _isChasing{ true };		//추적 상태 여부

		//보스 행동 로직 테스트를 위한 변수
		//팔요시 사용하나 제거해도 무관.
		bool _isRotatingToPlayer{ true };
		float _rotateToPlayerTime = 0.f;

		bool _isEvading{ false };
		bool _hasEvaded{ false };
		float _evadeCooldownTime = 0.f;

	};
}

