#pragma once
#include "MonsterState.h"
#include <unordered_map>

namespace Pg::Data
{
	/// Boss
	struct Boss
	{
		Pg::Data::BossState _bossState = Pg::Data::BossState::IDLE;
		Pg::Data::BossPase _bossPase = Pg::Data::BossPase::PASE_1;

		std::unordered_map<std::string, Pg::Data::BossState> _bossStateListByString;
		std::unordered_map<Pg::Data::BossState, std::string> _bossStateListByEnum;

		bool _isPase_1;					// 보스의 1페이즈
		bool _isPase_2;					// 보스의 1페이즈
		bool _isPase_3;					// 보스의 1페이즈

		bool _isDown;					// 보스가 무력화 되었는지
		bool _isDash;					// 보스가 대쉬를 했는지
	};


	/// Golem
	struct GolemBoss
	{
		Pg::Data::GolemBossState _bossState;
		Pg::Data::BossPase _bossPase;

		bool _isPase_1;					// 중간보스의 1페이즈
		bool _isPase_2;					// 중간보스의 1페이즈
		bool _isPase_3;					// 중간보스의 1페이즈

		bool _isSkill;					// 스킬 사용 여부
		bool _isDown;					// 스킬 사용 여부

		bool _isDash;					// 골렘 대쉬
	};	
	
	struct MiniGolem
	{
		bool _isDash;					// 골렘 대쉬
	};

	/// Trent
	struct Stub
	{
		Pg::Data::StubState _stubState; // 그루터기의 현재 상태(행동패턴)
	};

	/// Mimic
	struct Mimic
	{
		bool _isTouched;				// 플레이어가 미믹과 상호작용 했는지 
	};

	/// Wasp
	struct Wasp
	{
		int _attackCount;				// 공격 횟수(스킬 사용을 위한)
	};
}