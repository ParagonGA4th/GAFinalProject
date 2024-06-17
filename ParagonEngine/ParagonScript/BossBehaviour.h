#pragma once
#include "ScriptInterface.h"
#include "BossInfo.h"
#include "BaseMonster.h"

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class CapsuleCollider;
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
	class BossBehaviour : public ScriptInterface<BossBehaviour>, public BaseMonsterInfo
	{	
		DEFINE_PARAGON_SCRIPT(BossBehaviour);
	public:
		BossBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//플레이어를 쫓는 함수
		void Chase();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//특정 방향을 향해 돌진.
		//미니골렘과는 달리 플레이어를 향하는 것이 아니라 자신이 보는 방향을 향해 돌진한다.
		void Dash();

		//플레이어를 공격.
		void Attack();

		//회피하는 로직.
		void Avoid();

		//보스의 무력화 상태
		void neutralize();

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

	public:
		//골렘보스의 상태와 수치에 대한 정보.
		BossInfo* _bossInfo;

		bool _isRotateFinish;

		//대쉬 관련 변수.
		bool _isDash;			//돌진 여부
		bool _hasDashed;		//돌진했는지 여부
	};
}

