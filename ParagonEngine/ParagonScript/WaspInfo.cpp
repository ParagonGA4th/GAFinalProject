#include "WaspInfo.h"

namespace Pg::DataScript
{
	WaspInfo::WaspInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(1.f), _evadeSpeed(5.f), _attackRange(10.f),
		_sightRange(15.f), _evadeRange(14.f), _attackDuration(3.f),
		_attackSpeed(3.f), _startAttackTime(1.f), _currentAttackTime(0.f)
	{

	}

	void WaspInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void WaspInfo::ResetAll()
	{

	}
}