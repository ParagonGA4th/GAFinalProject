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

		//�÷��̾� �߰����� ������ �ϴ� �ൿ
		void Idle();

		//Ÿ���� ��ġ�� ���� �ٶ�.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//�÷��̾ ����.
		void Attack(bool _isAttack);

		//�׷��ͱ��� ��ų����
		void Skill(bool _isSkill);

		//�ǰ� �� �ִϸ��̼� ����� ���� �Լ�.
		void Hit();

		//�ǰ� �� ����.
		void Dead();
		
	public:
		BEGIN_VISITABLES(Pg::DataScript::StubBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
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
		//�÷��̾���� �Ÿ� ����
		float _distance;

		//���� ������ ���� ī��Ʈ
		int _attackCount;

		bool _isRotateFinish;
		bool _isFindSoundPlaying{ false };
		bool _isAttackSoundPlaying{ false };
		bool _isSkillSoundPlaying{ false };
		bool _isAnimStartInit{ false };

	public:
		//�׷��ͱ��� ���¿� ��ġ�� ���� ����.
		StubInfo* _stubInfo;

	};
}

