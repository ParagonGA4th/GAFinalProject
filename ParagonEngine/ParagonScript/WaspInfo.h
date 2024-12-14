#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//미니골렘의 상태
enum class WaspStatus
{
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적
	DASH,				// 돌진

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
};

/// <summary>
/// 미니골렘의 정보
/// </summary>
namespace Pg::DataScript
{
	class WaspInfo : public BaseMonsterInfo
	{
	public:
		WaspInfo(float fullHealth, float attackPower);

		//플레이어와의 이벤트가 발생할 때 쓰면 된다.
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

		WaspStatus _status; // 상태

	private:
		float _armor;          // 방어력

		float _sightRange;		//시야 범위
		float _attackRange;    // 기본 공격 범위
		float _evadeRange;		//회피 범위
		float _moveSpeed;      // 이동속도
		float _rotationSpeed;  // 회전 속도
		float _skillSpeed;		//스킬 속도

		float _attackDuration;	//공격 지속 시간
		float _attackSpeed;		//투사체 속도

		float _startAttackTime;
		float _currentAttackTime;

		float _startSkillTime;
		float _currentSkillTime;
		float _skillDuration;
	};
}


