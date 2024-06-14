#pragma once
///모든 몬스터에 대한 상태와 정보를 담아주는 헤더.
///

// 몬스터의 상태

enum class BossGolemStatus
{
	ENTER,				// 조우 시 플레이어 마주치기 전
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적
	RUSH,				// 돌진

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
	SKILL_ATTACK,		// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
	PATTERN_END			// 패턴이 끝난 경우
};

//미니골렘들의 스탯 정보
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


	float _hp;				// 체력

	float _armor;			// 방어력
	float _damage;			// 데미지
	float _attackRange;		// 기본 공격 범위

	float _moveSpeed;		// 이동속도
	float _rotationSpeed;	// 회전 속도

private:
	float _maxHp;				// 최대 체력
};

//보스골렘들의 스탯 정보
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

	float _hp;				// 체력

	float _armor;			// 방어력
	float _damage;			// 데미지
	float _attackRange;		// 기본 공격 범위

	float _moveSpeed;		// 이동속도
	float _rotationSpeed;	// 회전 속도

private:
	float _maxHp;				// 최대 체력
};