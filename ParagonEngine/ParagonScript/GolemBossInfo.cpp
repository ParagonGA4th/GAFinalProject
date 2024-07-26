#include "GolemBossInfo.h"

namespace Pg::DataScript
{
	GolemBossInfo::GolemBossInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(3.f), _dashSpeed(2.5f), _attackRange(5.f),
		_sightRange(15.f), _dashRange(15.f), _dashDuration(1.f),
		_currentDown(0.f), _endDown(3.f)
	{

	}

	void GolemBossInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void GolemBossInfo::ResetAll()
	{
		_hp = _fullHealthValue;
	}
}