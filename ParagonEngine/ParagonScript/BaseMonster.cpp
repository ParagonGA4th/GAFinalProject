#include "BaseMonster.h"
#include <algorithm>

namespace Pg::DataScript
{
	BaseMonster::BaseMonster(float fullHealth, float attackPower) : 
		_health(fullHealth), _fullHealthValue(fullHealth), _attackPower(attackPower)
	{
		
	}

	void BaseMonster::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		assert(false && "이거 자체로 호출되면 오류다!");
	}

	void BaseMonster::ResetAll()
	{
		assert(false && "이거 자체로 호출되면 오류다.");
	}

	void BaseMonster::ChangeMonsterHealth(float level)
	{
		_health = std::clamp<float>(_health + level, 0.0f, _fullHealthValue);
	}
	

	//

}