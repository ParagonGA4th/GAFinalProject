#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "StubInfo.h"
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
	class StubBehaviour : public ScriptInterface<StubBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(StubBehaviour);
	public:
		StubBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override;

		//플레이어 발견하지 않을때 하는 행동
		void Idle();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//플레이어를 공격.
		void Attack(bool _isAttack);

		//그루터기의 스킬공격
		void Skill(bool _isSkill);

		//피격 시 애니메이션 출력을 위한 함수.
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
		Pg::Data::BoxCollider* _collider;

		std::vector<Pg::Data::StaticBoxCollider*> _basicAttackCol;
		std::vector<Pg::Data::StaticBoxCollider*> _skillAttackCol;

	private:
		Pg::Data::GameObject* _stubHit;
		Pg::Data::AudioSource* _hitSound;

		Pg::Data::GameObject* _stubDie;
		Pg::Data::AudioSource* _dieSound;

		Pg::Data::GameObject* _stubSkill;
		Pg::Data::AudioSource* _skillSound;

		Pg::Data::GameObject* _stubAttack;
		Pg::Data::AudioSource* _attackSound;
	private:
		//플레이어와의 거리 측정
		float _distance;
		float _startAttackTime;
		float _endAttackTime;
		float _currentAttackTime;

		//공격 패턴을 위한 카운트
		int _attackCount;

		bool _isRotateFinish;

	public:
		//그루터기의 상태와 수치에 대한 정보.
		StubInfo* _stubInfo;

	};
}

