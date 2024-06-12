#include "StubInfo.h"

namespace Pg::DataScript
{
	StubInfo::StubInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(0.2f), _attackRange(3.f), _sightRange(15.f)
	{

	}

	void StubInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void StubInfo::ResetAll()
	{

	}
}
