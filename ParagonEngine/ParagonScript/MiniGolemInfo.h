#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//�̴ϰ��� ����
enum class MiniGolemStatus
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
	class MiniGolemInfo : public BaseMonsterInfo
	{
	public:
		MiniGolemInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		MiniGolemInfo& SetArmor(float armor) { _armor = armor; return *this; }
		MiniGolemInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		MiniGolemInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		MiniGolemInfo& SetCurrentDashTime(float time) { _cureentDashTime = time; return *this; }
		MiniGolemInfo& SetSightRange(float range) { _sightRange = range; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetDashSpeed() const { return _dashSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetDashRange() const { return _dashRange; }
		float GetDashDuration() const { return _dashDuration; }
		float GetCurrentDashTime() { return _cureentDashTime; }

		MiniGolemStatus _status; // ����

	private:
		float _armor;          // ����

		float _sightRange;		//�þ� ����
		float _attackRange;    // �⺻ ���� ����
		float _dashRange;		//���� ����
		float _moveSpeed;      // �̵��ӵ�
		float _rotationSpeed;  // ȸ�� �ӵ�
		float _dashSpeed;		// ���� �ӵ�

		float _dashDuration;	//���� ���� �ð�
		float _cureentDashTime; //���� ���� �ð�
	};
}


