#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//�̴ϰ��� ����
enum class WaspStatus
{
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����
	DASH,				// ����

	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
};

/// <summary>
/// �̴ϰ��� ����
/// </summary>
namespace Pg::DataScript
{
	class WaspInfo : public BaseMonsterInfo
	{
	public:
		WaspInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		WaspInfo& SetArmor(float armor) { _armor = armor; return *this; }
		WaspInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		WaspInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		WaspInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		WaspInfo& SetStartAttackTime(float time) { _startAttackTime = time; return *this; }
		WaspInfo& SetStartSkillTime(float time) { _startSkillTime = time; return *this; }
		WaspInfo& SetCurrentAttackTime(float time) { _currentAttackTime = time; return *this; }
		WaspInfo& SetCurrentSkillTime(float time) { _currentSkillTime = time; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetSkillSpeed() const { return _skillSpeed; }
		float GetAttackSpeed() const { return _attackSpeed; }

		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetEvadeRange() const { return _evadeRange; }

		float GetAttackDuration() const { return _attackDuration; }
		float GetStartAttackTime() const { return _startAttackTime; }
		float GetCurrentAttackTime() const { return _currentAttackTime; }

		float GetSkillDuration() const { return _skillDuration; }
		float GetStartSkillTime() const { return _startSkillTime; }
		float GetCurrentSkillTime() const { return _currentSkillTime; }

		WaspStatus _status; // ����

	private:
		float _armor;          // ����

		float _sightRange;		//�þ� ����
		float _attackRange;    // �⺻ ���� ����
		float _evadeRange;		//ȸ�� ����
		float _moveSpeed;      // �̵��ӵ�
		float _rotationSpeed;  // ȸ�� �ӵ�
		float _skillSpeed;		//��ų �ӵ�

		float _attackDuration;	//���� ���� �ð�
		float _attackSpeed;		//����ü �ӵ�

		float _startAttackTime;
		float _currentAttackTime;

		float _startSkillTime;
		float _currentSkillTime;
		float _skillDuration;
	};
}


