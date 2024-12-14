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


		//�÷��̾� �߰����� ������ �ϴ� �ൿ
		void Idle();

		//�÷��̾ �Ѵ� �Լ�
		void Chase();

		//�����ϴ� �Լ�
		void Dash();

		//Ÿ���� ��ġ�� ���� �ٶ�.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		// ���� ������� �Ѿ�� ����
		void Down();

		//�÷��̾ ����.
		void Attack(bool _isAttack);
		void Skill(bool _isSkill);

		//�ǰ� �� �ִϸ��̼� ����� ���� �Լ�.
		void Hit();

		//�ǰ� �� ����.
		void Dead();

	public:
		BEGIN_VISITABLES(Pg::DataScript::GolemBossBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
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

		//���Ͱ� ������ �� ��ġ
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
		//�÷��̾���� �Ÿ� ����
		float _distance;

		//������ ����
		bool _isRotateFinish;

		//�뽬 ���� ����.
		bool _isDash;			//���� ����
		bool _hasDashed;		//�����ߴ��� ����

		bool _isDashSoundPlaying = false; //���� �Ҹ�
		bool _isAttackSoundPlaying = false; //���� �Ҹ�
		bool _isSpinAttackSoundPlaying = false; //���� �Ҹ�
		
		// down ����
		bool _isDownInit;
		bool _isDown;

		bool _rotateToPlayer{ false };

		bool _isAnimChange{ false };
		bool animLoop{ false };
		std::string animId;

	public:
		//�̴ϰ��� ���¿� ��ġ�� ���� ����.
		GolemBossInfo* _golBossInfo;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };
	};
}

