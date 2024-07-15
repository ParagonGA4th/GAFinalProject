#include "BaseMonster.h"
#include <algorithm>

namespace Pg::DataScript
{
	BaseMonsterInfo::BaseMonsterInfo(float fullHealth, float attackPower) :
		_hp(fullHealth), _fullHealthValue(fullHealth), _damage(attackPower)
	{
		
	}

	void BaseMonsterInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		assert(false && "이거 자체로 호출되면 오류다!");
	}

	void BaseMonsterInfo::ResetAll()
	{
		assert(false && "이거 자체로 호출되면 오류다.");
	}

	void BaseMonsterInfo::ChangeMonsterHp(float level)
	{
		_hp = std::clamp<float>(_hp + level, 0.0f, _fullHealthValue);

		//죽으면 OnDead 호출.
		if (_hp <= std::numeric_limits<float>::epsilon())
		{
			_onDead();
		}
	}

	void BaseMonsterInfo::SetMonsterDamage(float damage)
	{
		_damage = damage;
	}

	void BaseMonsterInfo::SetMonsterHp(float hp)
	{
		_hp = hp;
	}

	float BaseMonsterInfo::GetMonsterHp()
	{
		return _hp;
	}

	float BaseMonsterInfo::GetMonsterDamage()
	{
		return _damage;
	}

	bool BaseMonsterInfo::IsMonsterDead()
	{
		return _hp <= std::numeric_limits<float>::epsilon();
	}

	void BaseMonsterInfo::SetMonsterSpeedRatio(float speed)
	{
		_speed = speed;
	}

	float BaseMonsterInfo::GetMonsterSpeedRatio()
	{
		return _speed;
	}

}