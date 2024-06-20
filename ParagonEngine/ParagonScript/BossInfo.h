#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//미니골렘의 상태
enum class BossStatus
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
	class BossInfo : public BaseMonsterInfo
	{
	public:
		BossInfo(float fullHealth, float attackPower);

		//플레이어와의 이벤트가 발생할 때 쓰면 된다.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		BossInfo& SetArmor(float armor) { _armor = armor; return *this; }
		BossInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		BossInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		BossInfo& SetCurrentDashTime(float time) { _cureentDashTime = time; return *this; }
		BossInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		BossInfo& SetCurrentNeutralize(float time) { _currentNeutralize = time; return *this; }
		BossInfo& SetEndNeutralize(float time) { _endNeutralize = time; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetDashSpeed() const { return _dashSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetDashRange() const { return _dashRange; }
		float GetDashDuration() const { return _dashDuration; }
		float GetCurrentDashTime() { return _cureentDashTime; }
		float GetCurrentNeutralize() { return _currentNeutralize; }
		float GetEndNeutralize() { return _endNeutralize; }

		BossStatus _status; // 상태

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

		float _currentNeutralize; //무력화 지속 시간
		float _endNeutralize; //무력화 지속 시간
	};
}


