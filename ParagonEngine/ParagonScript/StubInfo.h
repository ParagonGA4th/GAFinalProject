#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//미니골렘의 상태
enum class StubStatus
{
	ENTER,				// 조우 시 플레이어 마주치기 전
	IDLE,				// 다음 공격 패턴을 정함

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
	SKILL_ATTACK,		// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
	SKILLCOOLDOWN,		// 스킬 쿨타임
	BASICCOOLDOWN		// 기본 공격 쿨타임
};

/// <summary>
/// 그루터기의 정보
/// </summary>
namespace Pg::DataScript
{
	class StubInfo : public Pg::DataScript::BaseMonsterInfo
	{
	public:
		StubInfo(float fullHealth, float attackPower);

		//플레이어와의 이벤트가 발생할 때 쓰면 된다.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		StubInfo& SetArmor(float armor) { _armor = armor; return *this; }
		StubInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }

		float GetAttackRange() const { return _attackRange; }

		StubStatus _status; // 상태

	private:
		float _armor;          // 방어력

		float _attackRange;    // 기본 공격 범위
		float _rotationSpeed;  // 회전 속도
	};
}


