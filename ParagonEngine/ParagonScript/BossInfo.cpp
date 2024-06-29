#include "BossInfo.h"

namespace Pg::DataScript
{
	BossInfo::BossInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(2.f), _dashSpeed(10.0f), _attackRange(3.f),
		_sightRange(15.f), _dashRange(14.f), _dashDuration(2.f),
		_currentNeutralize(0.f), _endNeutralize(3.f), _currentEvadeTime(0.f),
		_evadeDuration(1.5f), _evadeSpeed(10.0f), _windBlastDuration(2.f),
		_windBlastSpeed(5.f), _startWindBlastTime(0.f)
	{

	}

	void BossInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void BossInfo::ResetAll()
	{

	}
}