#include "BossInfo.h"

namespace Pg::DataScript
{
	BossInfo::BossInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(0.4f), _dashSpeed(1.4f), _attackRange(4.f),
		_sightRange(15.f), _dashRange(14.f), _dashDuration(1.f),
		_currentNeutralize(0.f), _endNeutralize(3.f)
	{

	}

	void BossInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void BossInfo::ResetAll()
	{

	}
}