#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "IScriptResettable.h"
#include "WaspInfo.h"

#include <vector>
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class StaticMeshRenderer;
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
	class BaseEnemyHandler;
	class PlayerHandler;
	class CameraShake;
	class WaspAttack;
	class WaspSkillAttack;

	class WaspBehaviour : public ScriptInterface<WaspBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(WaspBehaviour);
	public:
		WaspBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		virtual void ResetAll() override;

		//플레이어를 쫓는 함수
		void Chase();

		//플레이어를 공격.
		void UpdateAttack();

		void UpdateSkillAttack();

		//피격 시 애니메이션 출력을 위한 함수.
		void Hit();

		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//피격 시 죽음.
		void Dead();
	public:
		BEGIN_VISITABLES(Pg::DataScript::WaspBehaviour);
		VISITABLE(int, _areaIndex);
		VISITABLE(std::string, _cornName);
		VISITABLE(std::string, _skillCornName);
		END_VISITABLES;

	public:
		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _waspInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; } //자신이 속해 있는 Area Index를 반환한다.

	public:
		//미니골렘의 상태와 수치에 대한 정보.
		WaspInfo* _waspInfo;

	private:
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;
		Pg::Data::CapsuleCollider* _collider;

		Pg::Data::GameObject* _player;
		Pg::Data::Transform* _playerTransform;
		Pg::Data::MonsterHelper* _monsterHelper;

		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::SkinnedMeshRenderer* _wingMeshRenderer;

		std::vector<Pg::Data::StaticBoxCollider*> _basicAttackCol;
		std::vector<Pg::Data::StaticBoxCollider*> _skillAttackCol;
	
	private:

		Pg::Data::AudioSource* _moveSound;
		Pg::Data::AudioSource* _hitSound;
		Pg::Data::AudioSource* _dieSound;
		Pg::Data::AudioSource* _dashSound;
		Pg::Data::GameObject* _waspSkillAttack;
		Pg::Data::AudioSource* _attackSound_1;
		Pg::Data::AudioSource* _attackSound_2;

		Pg::Data::GameObject* _corn;
		Pg::Data::StaticMeshRenderer* _cornRenderer;

		Pg::Data::GameObject* _skillCorn;
		Pg::Data::StaticMeshRenderer* _skillCornRenderer;

		CameraShake* _cameraShake;
		WaspAttack* _waspAttackScript;
		WaspSkillAttack* _waspSkillAttackScript;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };

	private:
		//플레이어와의 거리 측정
		float _distance;

		//사툰드 관련 변수
		bool _isAttackSoundPlaying{ false };
		bool _isSkillAttackSoundPlaying{ false };
		bool _isMoveSoundPlaying{ false };

		//공격 관련 변수
		bool _isAttackStart{ false };

		bool _isRotateToPlayer{ false };

		bool _isSkillStart{ false };
	};
}

