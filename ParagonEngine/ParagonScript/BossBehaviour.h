#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "IScriptResettable.h"
#include "BossInfo.h"
#include "BaseMonster.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class CapsuleCollider;
	class StaticBoxCollider;
	class StaticSphereCollider;
	class MonsterHelper;
	class AudioSource;
}

namespace Pg::API
{
	class PgScene;

	namespace Time
	{
		class PgTime;
	}

	namespace Tween
	{
		class PgTween;
	}
}

namespace Pg::Util
{
	class Tween;
}

namespace Pg::DataScript
{
	class CombatSystem;
	class CameraShake;
	class BaseEnemyHandler;

	class BossBehaviour : public ScriptInterface<BossBehaviour>, public IEnemyBehaviour	
	{
		DEFINE_PARAGON_SCRIPT(BossBehaviour);
	public:
		inline static const float MAX_BOSS_HEALTH = 400.f;
		inline static const float BOSS_ATTACK_POWER = 4.f;
	public:
		BossBehaviour(Pg::Data::GameObject* obj);
		virtual void BeforePhysicsAwake() override;
		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		virtual void ResetAll() override;

		//�÷��̾ �Ѵ� �Լ�
		void Chase();

		//Ÿ���� ��ġ�� ���� �ٶ�.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//Ư�� ������ ���� ����.
		//�̴ϰ񷽰��� �޸� �÷��̾ ���ϴ� ���� �ƴ϶� �ڽ��� ���� ������ ���� �����Ѵ�.
		void Dash();

		//�÷��̾ ����.
		void Attack(bool _isAttack);

		//������ ��ų ���ݵ�.
		void UpdateSkill();
		void UpdatePhaseTwoSkill();
		void UpdatePhaseThreeSkill();

		//ȸ���ϴ� ����.
		void Evade();

		//������ ����ȭ ����
		void Neutralize();

		//�ǰ� �� �ൿ.
		void Hit();

		//�ǰ� �� ����.
		void Dead();

		float RandomRange(float min, float max);

		bool GetProhibitAttack() { return _prohibitAttack; }

	public:
		BEGIN_VISITABLES(Pg::DataScript::BossBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _bossInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; }

		inline static const float  ALPHA_PERCENT  = 6.f;
		inline static const float  DASH_COOLTIME = 0.3f;

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::CapsuleCollider* _collider;

		Pg::Data::GameObject* _wind;
		Pg::Data::SkinnedMeshRenderer* _windRenderer;

		std::vector<Pg::Data::SkinnedMeshRenderer*> _lightSkillRenderer;

		std::vector<Pg::Data::StaticBoxCollider*> _basicAttackCol;
		std::vector<Pg::Data::StaticBoxCollider*> _windBlastAttackCol;
		std::vector<Pg::Data::StaticBoxCollider*> _lightAttackCol;
		std::vector<Pg::Data::StaticSphereCollider*> _takeDownCol;

		Pg::Data::AudioSource* _walkAudio;
		Pg::Data::AudioSource* _rushAudio;
		Pg::Data::AudioSource* _dieAudio;
		Pg::Data::AudioSource* _downAudio;
		Pg::Data::AudioSource* _hitAudio;
		Pg::Data::AudioSource* _basicAttackAudio1;
		Pg::Data::AudioSource* _basicAttackAudio2;
		Pg::Data::AudioSource* _laserAttack;
		Pg::Data::AudioSource* _upSound;
		Pg::Data::AudioSource* _downSound;

		Pg::Math::PGFLOAT3 _rendererOffset = { 0.f, -4.f, 0.f };

		Pg::Util::Tween* _riseTween;
		Pg::Util::Tween* _fallTween;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };

	public:
		//�񷽺����� ���¿� ��ġ�� ���� ����.
		BossInfo* _bossInfo;

		CombatSystem* _combatSystem;
		CameraShake* _cameraShake;
	private:
		float _distance;				//������ �÷��̾���� �Ÿ�
		bool _isPlayerInit{ false };

		bool _isRotateFinish;

		//�뽬 ���� ����.
		bool _isDash{ false };			//���� ����
		bool _hasDashed{ false };		//�����ߴ��� ����
		int _dashCount{ 0 };			//�� �� ���� �ߴ���
		float _dashCoolTime = DASH_COOLTIME;	//���� �� ���� ������ ���� 

		bool _isNeutralizeInit{ false };	//����ȭ ����
		bool _isNeutralize{ false };		//����ȭ ���� ����

		bool _isChasing{ true };		//���� ���� ����

		//���� �ൿ ���� �׽�Ʈ�� ���� ����
		//�ȿ�� ����ϳ� �����ص� ����.
		bool _isRotatingToPlayer{ true };
		float _rotateToPlayerTime = 0.f;

		bool _isEvading{ false };
		bool _hasEvaded{ false };
		float _evadeCooldownTime = 0.f;

		bool _isMoving{ false };
		bool _isRushSoundPlaying{ false };

		bool _isDeadInit{ false };

		bool _useStormBlast{ false };
		bool _offWind{ false };

		bool _useLightSkill{ false };
		
		bool _useTakeDownSkill{ false };

		bool _goUp{ false };
		bool _isGenerateCol{ false };

		bool _isRiseTween{ false };
		bool _isFallTween{ false };

		bool _isDownSoundPlaying{ false };
		bool _isAttackSoundPlaying{ false };
		bool _isStormAttackSoundPlaying{ false };
		bool _isLightSkillSoundPlaying{ false };

		//�÷��̾��� ������ ������ ��
		bool _prohibitAttack{ false };
		bool _takeDownFinish{ false };

		float _activationInterval{ 1.f }; // �� �ݶ��̴��� Ȱ��ȭ�Ǵ� �ð� ����
		float _nextActivationTime{ 0.0f }; // ���� �ݶ��̴��� Ȱ��ȭ�� �ð�
		size_t _currentColIndex = 0;      // ���� Ȱ��ȭ�� �ݶ��̴��� �ε���
	};
}

