#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//샤치의 상태
enum class BossStatus
{
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적
	DASH,				// 돌진
	EVADE,				// 회피

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
};

/// <summary>
/// 샤치의 정보
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
		BossInfo& SetCurrentEvadeTime(float time) { _currentEvadeTime = time; return *this; }
		BossInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		BossInfo& SetCurrentNeutralize(float time) { _currentNeutralize = time; return *this; }
		BossInfo& SetEndNeutralize(float time) { _endNeutralize = time; return *this; }
		BossInfo& SetWindBlastDuration(float time) { _windBlastDuration = time; return *this; }
		BossInfo& SetWindBlastSpeed(float speed) { _windBlastSpeed = speed; return *this; }
		BossInfo& SetStartWindBlastDurationTime(float time) { _startWindBlastTime = time; return *this; }
		BossInfo& SetCurrentWindBlastDurationTime(float time) { _currentWindBlastTime = time; return *this; }
		BossInfo& SetLightSkillDurationTime(float time) { _lightSkillDuration = time; return *this; }
		BossInfo& SetStartLightSkillTime(float time) { _startLightSkillTime = time; return *this; }
		BossInfo& SetCurrentLightSkillTime(float time) { _currentLightSkillTime = time; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetDashSpeed() const { return _dashSpeed; }
		float GetEvadeSpeed() const { return _evadeSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetDashRange() const { return _dashRange; }
		float GetDashDuration() const { return _dashDuration; }
		float GetEvadeDuration() const { return _evadeDuration; }
		float GetCurrentDashTime() { return _cureentDashTime; }
		float GetCurrentEvadeTime() { return _currentEvadeTime; }
		float GetCurrentNeutralize() { return _currentNeutralize; }
		float GetEndNeutralize() { return _endNeutralize; }
		float GetWindBlastDuration() { return _windBlastDuration; }
		float GetWindBlastSpeed() { return _windBlastSpeed; }
		float GetStartWindBlastTime() { return _startWindBlastTime; }
		float GetCurrentWindBlastTime() { return _currentWindBlastTime; }
		float GetLightSkillDuration() { return _lightSkillDuration; }
		float GetStartLightSkillTime() { return _startLightSkillTime; }
		float GetCurrentLightSkillTime() { return _currentLightSkillTime; }

		BossStatus _status; // 상태

	private:
		float _armor;          // 방어력

		float _sightRange;		//시야 범위
		float _attackRange;    // 기본 공격 범위
		float _dashRange;		//돌진 범위
		float _moveSpeed;      // 이동속도
		float _rotationSpeed;  // 회전 속도
		float _dashSpeed;		// 돌진 속도
		float _evadeSpeed;		// 회피 속도

		float _dashDuration;	//돌진 지속 시간
		float _cureentDashTime; //현재 돌진 시간

		float _evadeDuration;	//회피 지속 시간
		float _currentEvadeTime;//현재 회피 시간

		float _currentNeutralize; //무력화 지속 시간
		float _endNeutralize; //무력화 지속 시간

		float _windBlastDuration; //돌풍 지속 시간
		float _windBlastSpeed; //돌풍 속도
		float _startWindBlastTime; //돌풍 시작 시간
		float _currentWindBlastTime;

		float _lightSkillDuration; //빛 스킬 지속 시간
		float _startLightSkillTime; //빛 스킬 시작 시간
		float _currentLightSkillTime; //현재 빛 스킬 시간
	};
}


