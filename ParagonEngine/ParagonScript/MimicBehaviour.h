#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "MimicInfo.h"
#include "BaseMonster.h"

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class BoxCollider;
	class MonsterHelper;
	class StaticBoxCollider;
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
	class MimicBehaviour : public ScriptInterface<MimicBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(MimicBehaviour);
	public:
		MimicBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override;

		//플레이어 발견하지 않을때 하는 행동
		void Idle();

		//플레이어를 쫓는 함수
		void Chase();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);
		
		//공격 함수
		void Attack(bool _isAttack);

		//사망 시 호출되는 함수
		void Dead();

		//피격 시 호출되는 함수
		void Hit();

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::BoxCollider* _collider;

		std::vector<Pg::Data::StaticBoxCollider*> _attackCol;

		//몬스터가 리스폰 될 위치
		Pg::Math::PGFLOAT3 _respawnPos;

		//플레이어와의 거리 측정
		float _distance;
		float _startAttackTime;
		float _endAttackTime;
		float _currentAttackTime;

		//몬스터의 상태
		bool _isStart;
		bool _isHit;
		bool _isRotateFinish;

		//대쉬 관련 변수.
		bool _isDash;			//돌진 여부
		bool _hasDashed;		//돌진했는지 여부

	public:
		//미믹의 상태와 수치에 대한 정보.
		MimicInfo* _mimicInfo;
	};
}

