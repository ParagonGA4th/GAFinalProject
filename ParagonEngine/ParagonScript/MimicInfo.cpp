#include "MimicInfo.h"

namespace Pg::DataScript
{
	MimicInfo::MimicInfo(float fullHealth, float attackPower) : BaseMonsterInfo(fullHealth, attackPower),
		_moveSpeed(0.2f), _attackRange(4.f), _skillAttackRange(8.f),
		_sightRange(15.f), _startSkillTime(2.f),_skillDuration(4.f),
		_skillSpeed(5.f), _currentSkillTime(0.f)
	{

	}

	void MimicInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void MimicInfo::ResetAll()
	{
		//체력 초기화
		_hp = _fullHealthValue;
	}
}