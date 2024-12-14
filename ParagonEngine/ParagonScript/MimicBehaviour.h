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

		//�÷��̾� �߰����� ������ �ϴ� �ൿ
		void Idle();

		//�÷��̾ �Ѵ� �Լ�
		void Chase();

		//Ÿ���� ��ġ�� ���� �ٶ�.
		void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);
		
		//���� �Լ�
		void Attack(bool _isAttack);

		void UpdateSkill();

		//��� �� ȣ��Ǵ� �Լ�
		void Dead();

		//�ǰ� �� ȣ��Ǵ� �Լ�
		void Hit();

	public:
		BEGIN_VISITABLES(Pg::DataScript::MimicBehaviour);
		VISITABLE(int, _areaIndex);
		VISITABLE(std::string, _coinName);
		END_VISITABLES;

	public:
		//�÷��̾�� � ���������� �����ϱ� ����.
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

		//���Ͱ� ������ �� ��ġ
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
		//MimicActive�� ���� �ٽ� Ȱ��ȭ �����ֱ� ����.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::CapsuleCollider* _collider;

	private:
		BaseEnemyHandler* _enemyHandler{ nullptr };

	private:
		//�÷��̾���� �Ÿ� ����
		float _distance;
		float _startAttackTime;
		float _endAttackTime;
		float _currentAttackTime;

		//������ ����
		bool _isStart;
		bool _isHit;
		bool _isRotateFinish;

		//�뽬 ���� ����.
		bool _isDash;			//���� ����
		bool _hasDashed;		//�����ߴ��� ����

		bool _isMoving{ false };
		//bool _isChasing{ true };

		//��ų ��� ����
		bool _useCoinThrow{ false };

		//�̹��� ��׷ΰ� Ǯ���� �ȵ�.
		bool _isRotateToPlayer{ true };

		bool _isAttackSoundPlaying{ false };
		bool _isSkillSoundPlaying{ false };
	public:
		//�̹��� ���¿� ��ġ�� ���� ����.
		MimicInfo* _mimicInfo;
	};
}

