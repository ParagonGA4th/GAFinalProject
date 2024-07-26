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

		//플레이어를 쫓는 함수
		void Chase();

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//특정 방향을 향해 돌진.
		//미니골렘과는 달리 플레이어를 향하는 것이 아니라 자신이 보는 방향을 향해 돌진한다.
		void Dash();

		//플레이어를 공격.
		void Attack(bool _isAttack);

		//보스의 스킬 공격들.
		void UpdateSkill();
		void UpdatePhaseTwoSkill();
		void UpdatePhaseThreeSkill();

		//회피하는 로직.
		void Evade();

		//보스의 무력화 상태
		void Neutralize();

		//피격 시 행동.
		void Hit();

		//피격 시 죽음.
		void Dead();

		float RandomRange(float min, float max);

		bool GetProhibitAttack() { return _prohibitAttack; }

	public:
		BEGIN_VISITABLES(Pg::DataScript::BossBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
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
		//골렘보스의 상태와 수치에 대한 정보.
		BossInfo* _bossInfo;

		CombatSystem* _combatSystem;
		CameraShake* _cameraShake;
	private:
		float _distance;				//보스와 플레이어와의 거리
		bool _isPlayerInit{ false };

		bool _isRotateFinish;

		//대쉬 관련 변수.
		bool _isDash{ false };			//돌진 여부
		bool _hasDashed{ false };		//돌진했는지 여부
		int _dashCount{ 0 };			//몇 번 돌진 했는지
		float _dashCoolTime = DASH_COOLTIME;	//돌진 후 다음 돌진을 위해 

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

		//플레이어의 공격을 막고자 함
		bool _prohibitAttack{ false };
		bool _takeDownFinish{ false };

		float _activationInterval{ 1.f }; // 각 콜라이더가 활성화되는 시간 간격
		float _nextActivationTime{ 0.0f }; // 다음 콜라이더가 활성화될 시간
		size_t _currentColIndex = 0;      // 현재 활성화할 콜라이더의 인덱스
	};
}

