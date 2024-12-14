#include "WaspInfo.h"

namespace Pg::DataScript
{
	WaspInfo::WaspInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(1.f), _skillSpeed(7.f), _attackRange(7.f),
		_sightRange(15.f), _evadeRange(5.f), _attackDuration(3.f),
		_attackSpeed(5.f), _startAttackTime(0.5f), _currentAttackTime(0.f),
		_startSkillTime(1.5f), _currentSkillTime(0.f), _skillDuration(3.f)
	{

	}

	void WaspInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void WaspInfo::ResetAll()
	{
		_hp = _fullHealthValue;
	}
}