#pragma once
#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//미니골렘의 상태
enum class MimicStatus
{
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
	SKILL_ATTACK		// 스킬 공격
};

/// <summary>
/// 미니골렘의 정보
/// </summary>
namespace Pg::DataScript
{
	class MimicInfo : public BaseMonsterInfo
	{
	public:
		MimicInfo(float fullHealth, float attackPower);

		//플레이어와의 이벤트가 발생할 때 쓰면 된다.
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

		MimicStatus _status; // 상태

	private:
		float _armor;          // 방어력

		float _sightRange;		//시야 범위
		float _attackRange;    // 기본 공격 범위
		float _skillAttackRange;    // 스킬 공격 범위
		float _moveSpeed;      // 이동속도
		float _rotationSpeed;  // 회전 속도

		float _startSkillTime;
		float _currentSkillTime;
		float _skillDuration;
		float _skillSpeed;
	};
}



