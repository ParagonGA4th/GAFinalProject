#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "IScriptResettable.h"
#include "MimicInfo.h"
#include "BaseMonster.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class StaticMeshRenderer;
	class BoxCollider;
	class CapsuleCollider;
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
	class CameraShake;
	class MimicSkillAttack;
	class BaseEnemyHandler;

	class MimicBehaviour : public ScriptInterface<MimicBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(MimicBehaviour);
	public:
		MimicBehaviour(Pg::Data::GameObject* obj);

	public:
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

		//타겟의 위치를 향해 바라봄.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);
		
		//공격 함수
		void Attack(bool _isAttack);

		void UpdateSkill();

		//사망 시 호출되는 함수
		void Dead();

		//피격 시 호출되는 함수
		void Hit();

	public:
		BEGIN_VISITABLES(Pg::DataScript::MimicBehaviour);
		VISITABLE(int, _areaIndex);
		VISITABLE(std::string, _coinName);
		END_VISITABLES;

	public:
		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _mimicInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; }

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::GameObject* _coin;
		Pg::Data::StaticMeshRenderer* _coinRenderer;

		std::vector<Pg::Data::StaticBoxCollider*> _basicAttackCol;
		std::vector<Pg::Data::StaticBoxCollider*> _skillAttackCol;

		//몬스터가 리스폰 될 위치
		Pg::Math::PGFLOAT3 _respawnPos;
		Pg::Math::PGFLOAT3 _rendererOffset = {0.f, -2.f, 0.f};

		Pg::Data::GameObject* _mimicMoveSound;
		Pg::Data::AudioSource* _moveAudio;
		Pg::Data::AudioSource* _hitAudio;
		Pg::Data::AudioSource* _dieAudio;
		Pg::Data::AudioSource* _attackAudio;
		Pg::Data::AudioSource* _skillAudio;

		CameraShake* _cameraShake;
		MimicSkillAttack* _mimicSkillAttack;

	public:
		//MimicActive를 통해 다시 활성화 시켜주기 위함.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::CapsuleCollider* _collider;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };

	private:
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

		bool _isMoving{ false };
		//bool _isChasing{ true };

		//스킬 사용 여부
		bool _useCoinThrow{ false };

		//미믹은 어그로가 풀리면 안됨.
		bool _isRotateToPlayer{ true };

		bool _isAttackSoundPlaying{ false };
		bool _isSkillSoundPlaying{ false };
	public:
		//미믹의 상태와 수치에 대한 정보.
		MimicInfo* _mimicInfo;
	};
}

