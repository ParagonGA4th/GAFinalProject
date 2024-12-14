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

		//�÷��̾ �Ѵ� �Լ�
		void Chase();

		//�÷��̾ ����.
		void UpdateAttack();

		void UpdateSkillAttack();

		//�ǰ� �� �ִϸ��̼� ����� ���� �Լ�.
		void Hit();

		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

		//�ǰ� �� ����.
		void Dead();
	public:
		BEGIN_VISITABLES(Pg::DataScript::WaspBehaviour);
		VISITABLE(int, _areaIndex);
		VISITABLE(std::string, _cornName);
		VISITABLE(std::string, _skillCornName);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _waspInfo; }
		virtual unsigned int GetBelongAreaIndex() override { return _areaIndex; } //�ڽ��� ���� �ִ� Area Index�� ��ȯ�Ѵ�.

	public:
		//�̴ϰ��� ���¿� ��ġ�� ���� ����.
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
		//�÷��̾���� �Ÿ� ����
		float _distance;

		//������ ���� ����
		bool _isAttackSoundPlaying{ false };
		bool _isSkillAttackSoundPlaying{ false };
		bool _isMoveSoundPlaying{ false };

		//���� ���� ����
		bool _isAttackStart{ false };

		bool _isRotateToPlayer{ false };

		bool _isSkillStart{ false };
	};
}

