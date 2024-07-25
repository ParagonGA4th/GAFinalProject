#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "IScriptResettable.h"
#include "StubInfo.h"
#include "BaseMonster.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
	class BoxCollider;
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
	class BaseEnemyHandler;

	class StubBehaviour : public ScriptInterface<StubBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(StubBehaviour);
	public:
		StubBehaviour(Pg::Data::GameObject* obj);

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
		
	public:
		BEGIN_VISITABLES(Pg::DataScript::StubBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//플레이어에게 어떤 몬스터인지를 전달하기 위함.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _stubInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; }

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

		Pg::Math::PGFLOAT3 _rendererOffset = { 0.f, -1.f, 0.f };

	private:
		Pg::Data::AudioSource* _hitSound;
		Pg::Data::AudioSource* _dieSound;
		Pg::Data::AudioSource* _skillSound;
		Pg::Data::AudioSource* _attackSound;
		Pg::Data::AudioSource* _findSound;

		CameraShake* _cameraShake;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };

	private:
		//플레이어와의 거리 측정
		float _distance;

		//공격 패턴을 위한 카운트
		int _attackCount;

		bool _isRotateFinish;
		bool _isFindSoundPlaying{ false };
		bool _isAttackSoundPlaying{ false };
		bool _isSkillSoundPlaying{ false };
		bool _isAnimStartInit{ false };

	public:
		//그루터기의 상태와 수치에 대한 정보.
		StubInfo* _stubInfo;

	};
}

