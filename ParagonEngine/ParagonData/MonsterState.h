#pragma once
namespace Pg::Data
{
	enum class TrentState
	{
		IDLE,
		SKILL_ATTACK,
		BASIC_ATTACK_1,
		BASIC_ATTACK_2,
		BASIC_ATTACK_3,
		SKILL_COOLDOWN,
		BASICATTACK_COOLDOWN,
		DEAD
	};
}