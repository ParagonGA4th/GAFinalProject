#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//�̴ϰ��� ����
enum class StubStatus
{
	ENTER,				// ���� �� �÷��̾� ����ġ�� ��
	IDLE,				// ���� ���� ������ ����

	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
	SKILL_ATTACK,		// ��ų ���		// ��ų ������ ���� ���� ����
	SKILLCOOLDOWN,		// ��ų ��Ÿ��
	BASICCOOLDOWN		// �⺻ ���� ��Ÿ��
};

/// <summary>
/// �׷��ͱ��� ����
/// </summary>
namespace Pg::DataScript
{
	class StubInfo : public Pg::DataScript::BaseMonsterInfo
	{
	public:
		StubInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		StubInfo& SetArmor(float armor) { _armor = armor; return *this; }
		StubInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }

		float GetAttackRange() const { return _attackRange; }

		StubStatus _status; // ����

	private:
		float _armor;          // ����

		float _attackRange;    // �⺻ ���� ����
		float _rotationSpeed;  // ȸ�� �ӵ�
	};
}


