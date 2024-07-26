#include "BossInfo.h"

namespace Pg::DataScript
{
	BossInfo::BossInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(5.f), _dashSpeed(15.f), _attackRange(4.f),
		_sightRange(15.f), _dashRange(14.f), _dashDuration(1.f),
		_currentNeutralize(0.f), _endNeutralize(3.f), _currentEvadeTime(0.f), _cureentDashTime(0.f),
		_evadeDuration(0.8f), _evadeSpeed(5.0f), _windBlastDuration(1.5f), _currentWindBlastTime(0.f),
		_windBlastSpeed(10.f), _startWindBlastTime(0.5f), _lightSkillDuration(8.f),
		_startLightSkillTime(0.f), _currentLightSkillTime(0.f)
	{

	}

	void BossInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void BossInfo::ResetAll()
	{
		//여기에 게임이 재시작 시 모든 것이 초기화 되어야 한다.
		_hp = _fullHealthValue;
	}
}