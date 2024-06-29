#include "WaspInfo.h"

namespace Pg::DataScript
{
	WaspInfo::WaspInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(1.f), _dashSpeed(1.4f), _attackRange(5.f),
		_sightRange(15.f), _dashRange(14.f), _attackDuration(1.5f),
		_attackSpeed(3.f)
	{

	}

	void WaspInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void WaspInfo::ResetAll()
	{

	}
}