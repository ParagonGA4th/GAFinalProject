#pragma once
namespace Pg::Data
{
	// 공격 패턴이 있는 몬스터를 behaviortree의 node 를 통해 제어하기 위해

	enum class TrentState
	{
		IDLE,
		BASIC_ATTACK_1,
		BASIC_ATTACK_2,
		BASIC_ATTACK_3,
		BASICATTACK_COOLDOWN,
		SKILL_ATTACK,
		SKILL_COOLDOWN,
		DEAD
	};	
	
	enum class BossState
	{
		IDLE,
		DOWN,
		EVASION,
		DASH,
		CHASE,
		BASIC_ATTACK_1,
		BASIC_ATTACK_2,
		BASIC_ATTACK_3,
		SKILL_FEATHER_ATTACK,
		SKILL_FLY_ATTACK_1,
		SKILL_FLY_ATTACK_PREPARE_1,
		SKILL_FLY_ATTACK_2,
		SKILL_FLY_ATTACK_PREPARE_2,
		SKILL_FLY_ATTACK_3,
		SKILL_FLY_ATTACK_PREPARE_3,
		DEAD
	};

	enum class BossPase
	{
		PASE_1,
		PASE_2,
		PASE_3,
	};
}