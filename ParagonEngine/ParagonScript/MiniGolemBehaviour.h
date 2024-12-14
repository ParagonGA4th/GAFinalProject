#pragma once
#include "ScriptInterface.h"
#include "IEnemyBehaviour.h"
#include "MiniGolemInfo.h"
#include "IScriptResettable.h"
#include "BaseMonster.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class GameObject;
	class PhysicsCollision;
	class SkinnedMeshRenderer;
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
	class PlayerHandler;
	class BaseEnemyHandler;
	class CameraShake;

	class MiniGolemBehaviour : public ScriptInterface<MiniGolemBehaviour>, public IEnemyBehaviour
	{
		DEFINE_PARAGON_SCRIPT(MiniGolemBehaviour);
	public:
		MiniGolemBehaviour(Pg::Data::GameObject* obj);

	private:
		inline static const float START_ATTACK_TIME = 0.7f;
		inline static const float END_ATTACK_TIME = 2.7f;

	public:
		virtual void BeforePhysicsAwake() override;
		virtual void GrabManagedObjects() override;
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

		//�÷��̾ ����.
		void Attack(bool _isAttack);

		//�ǰ� �� �ִϸ��̼� ����� ���� �Լ�.
		void Hit();

		//�ǰ� �� ����.
		void Dead();

	public:
		BEGIN_VISITABLES(Pg::DataScript::MiniGolemBehaviour);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() override { return _miniGolInfo; }
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

		//���Ͱ� ������ �� ��ġ
		Pg::Math::PGFLOAT3 _respawnPos;

	private:
		Pg::Data::AudioSource* _hitSound;
		Pg::Data::AudioSource* _dieSound;
		Pg::Data::AudioSource* _dashSound;
		Pg::Data::AudioSource* _attackSound;

		CameraShake* _cameraShake;

	private:

		//�÷��̾���� �Ÿ� ����
		float _distance;
		float _currentAttackTime;

		//������ ����
		bool _isRotateFinish;

		//�뽬 ���� ����.
		bool _isDash;			//���� ����
		bool _hasDashed;		//�����ߴ��� ����

		bool _isDashSoundPlaying = false; //���� �Ҹ�
		bool _isAttackSoundPlaying = false; //���� �Ҹ�

	public:
		//�̴ϰ��� ���¿� ��ġ�� ���� ����.
		MiniGolemInfo* _miniGolInfo;
	
	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };
	};
}

