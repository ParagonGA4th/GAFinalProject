#pragma once
///��� ���Ϳ� ���� ���¿� ������ ����ִ� ���.
///

// ������ ����

enum class BossGolemStatus
{
	ENTER,				// ���� �� �÷��̾� ����ġ�� ��
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����
	RUSH,				// ����

	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
	SKILL_ATTACK,		// ��ų ���		// ��ų ������ ���� ���� ����
	PATTERN_END			// ������ ���� ���
};

//�̴ϰ񷽵��� ���� ����
struct MiniGolemInfo
{
	MiniGolemInfo()
		: _hp(0.0f), _armor(0.0f), _damage(0.0f), _attackRange(0.0f), _moveSpeed(20.0f), _rotationSpeed(100.0f),
		_maxHp(0.0f)
	{};

	MiniGolemInfo& SetHp(float hp) { _hp = hp;	_maxHp = hp;	return *this; };
	MiniGolemInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	MiniGolemInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	MiniGolemInfo& SetAttackRange(float attackRange) { _attackRange = attackRange;  return *this; };
	MiniGolemInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };
	
	float GetMaxHp() { return _maxHp; };


	float _hp;				// ü��

	float _armor;			// ����
	float _damage;			// ������
	float _attackRange;		// �⺻ ���� ����

	float _moveSpeed;		// �̵��ӵ�
	float _rotationSpeed;	// ȸ�� �ӵ�

private:
	float _maxHp;				// �ִ� ü��
};

//�����񷽵��� ���� ����
struct BossGolemInfo
{
	BossGolemInfo()
		: _hp(0.0f), _armor(0.0f), _damage(0.0f), _attackRange(0.0f), _moveSpeed(0.2f), _rotationSpeed(100.0f),
		_maxHp(0.0f)
	{};

	BossGolemInfo& SetHp(float hp) { _hp = hp;	_maxHp = hp;	return *this; };
	BossGolemInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	BossGolemInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	BossGolemInfo& SetAttackRange(float attackRange) { _attackRange = attackRange;  return *this; };
	BossGolemInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };

	float _hp;				// ü��

	float _armor;			// ����
	float _damage;			// ������
	float _attackRange;		// �⺻ ���� ����

	float _moveSpeed;		// �̵��ӵ�
	float _rotationSpeed;	// ȸ�� �ӵ�

private:
	float _maxHp;				// �ִ� ü��
};