#include "GolemBossInfo.h"

namespace Pg::DataScript
{
	GolemBossInfo::GolemBossInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(3.f), _dashSpeed(1.4f), _attackRange(5.f),
		_sightRange(15.f), _dashRange(14.f), _dashDuration(1.f)
	{

	}

	void GolemBossInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void GolemBossInfo::ResetAll()
	{

	}
}