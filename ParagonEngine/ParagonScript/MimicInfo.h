#pragma once
#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//�̴ϰ��� ����
enum class MimicStatus
{
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����

	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
	SKILL_ATTACK		// ��ų ����
};

/// <summary>
/// �̴ϰ��� ����
/// </summary>
namespace Pg::DataScript
{
	class MimicInfo : public BaseMonsterInfo
	{
	public:
		MimicInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		MimicInfo& SetArmor(float armor) { _armor = armor; return *this; }
		MimicInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		MimicInfo& SetSkillAttackRange(float attackRange) { _skillAttackRange = attackRange; return *this; }
		MimicInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		MimicInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		MimicInfo& SetStartSKillTime(float time) { _startSkillTime = time; return *this; }
		MimicInfo& SetCurrentSKillTime(float time) { _currentSkillTime = time; return *this; }
		MimicInfo& SetSkillDuration(float dur) { _skillDuration = dur; return *this; }
		MimicInfo& SetSkillSpeed(float speed) { _skillSpeed = speed; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetSkillAttackRange() const { return _skillAttackRange; }
		float GetStartSkillTime() const { return _startSkillTime; }
		float GetCurrentSkillTime() const { return _currentSkillTime; }
		float GetSkillDuration() const { return _skillDuration; }
		float GetSkillSpeed() const { return _skillSpeed; }

		MimicStatus _status; // ����

	private:
		float _armor;          // ����

		float _sightRange;		//�þ� ����
		float _attackRange;    // �⺻ ���� ����
		float _skillAttackRange;    // ��ų ���� ����
		float _moveSpeed;      // �̵��ӵ�
		float _rotationSpeed;  // ȸ�� �ӵ�

		float _startSkillTime;
		float _currentSkillTime;
		float _skillDuration;
		float _skillSpeed;
	};
}



