#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//미니골렘의 상태
enum class MiniGolemStatus
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
	class MiniGolemInfo : public BaseMonsterInfo
	{
	public:
		MiniGolemInfo(float fullHealth, float attackPower);

		//플레이어와의 이벤트가 발생할 때 쓰면 된다.
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

		MiniGolemStatus _status; // 상태

	private:
		float _armor;          // 방어력

		float _sightRange;		//시야 범위
		float _attackRange;    // 기본 공격 범위
		float _dashRange;		//돌진 범위
		float _moveSpeed;      // 이동속도
		float _rotationSpeed;  // 회전 속도
		float _dashSpeed;		// 돌진 속도

		float _dashDuration;	//돌진 지속 시간
		float _cureentDashTime; //현재 돌진 시간
	};
}


