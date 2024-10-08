#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "IScriptResettable.h"
#include "BaseMonster.h"
#include "GolemBossInfo.h"
#include "BaseMonster.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class CapsuleCollider;
	class StaticSphereCollider;
	class MonsterHelper;
	class StaticBoxCollider;
	class AudioSource;
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
	class PlayerHandler;
	class BaseEnemyHandler;
	class CameraShake;
	class CombatSystem;

	class GolemBossBehaviour : public ScriptInterface<GolemBossBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(GolemBossBehaviour);
	public:
		inline static const float MAX_GOLEM_BOSS_HEALTH = 100.f;
		inline static const float GOLEM_BOSS_ATTACK_POWER = 2.f;

	public:
		GolemBossBehaviour(Pg::Data::GameObject* obj);
		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		virtual void ResetAll() override;


		//플레이어 발견하지 않을때 하는 행동
		void Idle();

		//플레이어를 쫓는 함수
		void Chase();

		//돌진하는 함수
		void Dash();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		// 다음 페이즈로 넘어가기 위해
		void Down();

		//플레이어를 공격.
		void Attack(bool _isAttack);
		void Skill(bool _isSkill);

		//피격 시 애니메이션 출력을 위한 함수.
		void Hit();

		//피격 시 죽음.
		void Dead();

	public:
		BEGIN_VISITABLES(Pg::DataScript::GolemBossBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _golBossInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; }

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::CapsuleCollider* _collider;

		std::vector<Pg::Data::StaticBoxCollider*> _attackCol;
		std::vector<Pg::Data::StaticSphereCollider*> _skillAttackCol;

		//몬스터가 리스폰 될 위치
		Pg::Math::PGFLOAT3 _respawnPos;
		Pg::Math::PGFLOAT3 _rendererOffset = {0.f, -2.f, 0.f};

	private:
		Pg::Data::AudioSource* _hitSound;
		Pg::Data::AudioSource* _rushSound;
		Pg::Data::AudioSource* _dieSound;
		Pg::Data::AudioSource* _basicAttackSound;
		Pg::Data::AudioSource* _spinAttackSound;

		CameraShake* _cameraShake;
		CombatSystem* _combatSystem;

	private:
		//플레이어와의 거리 측정
		float _distance;

		//몬스터의 상태
		bool _isRotateFinish;

		//대쉬 관련 변수.
		bool _isDash;			//돌진 여부
		bool _hasDashed;		//돌진했는지 여부

		bool _isDashSoundPlaying = false; //돌진 소리
		bool _isAttackSoundPlaying = false; //공격 소리
		bool _isSpinAttackSoundPlaying = false; //공격 소리
		
		// down 관련
		bool _isDownInit;
		bool _isDown;

		bool _rotateToPlayer{ false };

		bool _isAnimChange{ false };
		bool animLoop{ false };
		std::string animId;

	public:
		//미니골렘의 상태와 수치에 대한 정보.
		GolemBossInfo* _golBossInfo;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };
	};
}

