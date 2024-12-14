#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//�̴ϰ��� ����
enum class GolemBossStatus
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
	class GolemBossInfo : public BaseMonsterInfo
	{
	public:
		GolemBossInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		GolemBossInfo& SetArmor(float armor) { _armor = armor; return *this; }
		GolemBossInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		GolemBossInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		GolemBossInfo& SetCurrentDashTime(float time) { _cureentDashTime = time; return *this; }
		GolemBossInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		GolemBossInfo& SetCurrentDown(float time) { _currentDown = time; return *this; }
		GolemBossInfo& SetEndDown(float time) { _endDown = time; return *this; }


		float GetMoveSpeed() const { return _moveSpeed; }
		float GetDashSpeed() const { return _dashSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetDashRange() const { return _dashRange; }
		float GetDashDuration() const { return _dashDuration; }
		float GetCurrentDashTime() { return _cureentDashTime; }		
		float GetEndDown() const { return _endDown; }
		float GetCurrentDown() { return _currentDown; }

		GolemBossStatus _status; // ����

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

		float _currentDown; //����ȭ ���� �ð�
		float _endDown;		//����ȭ ���� �ð�
	};
}



