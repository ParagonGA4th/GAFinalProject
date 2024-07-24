#pragma once
#include "MonsterState.h"
#include <unordered_map>

namespace Pg::Data
{
	/// Boss
	struct Boss
	{
		Pg::Data::BossState _bossState = Pg::Data::BossState::IDLE;
		Pg::Data::BossPase _bossPase = Pg::Data::BossPase::PASE_INIT;

		std::unordered_map<Pg::Data::BossState, std::string> _bossStateListByEnum;

		bool _isDownInit;				// 보스 무력화 시작
		bool _isDown;					// 보스가 무력화 되었는지
		bool _isDownEnd;				// 보스 무력화 종료

		void Reset()
		{
			_isDownInit = false;
			_isDown = false;
			_isDownEnd = false;

			_bossState = Pg::Data::BossState::IDLE;
			_bossPase = Pg::Data::BossPase::PASE_INIT;
		}
	};


	/// Golem
	struct GolemBoss
	{
		Pg::Data::GolemBossState _bossState;
		Pg::Data::BossPase _bossPase;

		std::unordered_map<Pg::Data::GolemBossState, std::string> _bossStateListByEnum;

		bool _isSkill;					// 스킬 사용 여부
		bool _isDown;					// 스킬 사용 여부

		bool _isDash;					// 골렘 대쉬

		void Reset()
		{
			_isSkill = false;
			_isDown = false;
			_isDash = false;

			_bossState = Pg::Data::GolemBossState::IDLE;
			_bossPase = Pg::Data::BossPase::PASE_1;
		}
	};	
	
	struct MiniGolem
	{
		bool _isDash;					// 골렘 대쉬

		void Reset()
		{
			_isDash = false;
		}
	};

	/// Trent
	struct Stub
	{
		Pg::Data::StubState _stubState = Pg::Data::StubState::IDLE; // 그루터기의 현재 상태(행동패턴)
	
		void Reset()
		{
			_stubState = Pg::Data::StubState::IDLE;
		}
	};

	/// Mimic
	struct Mimic
	{
		bool _isTouched;				// 플레이어가 미믹과 상호작용 했는지 

		void Reset()
		{
			_isTouched = false;
		}
	};

	/// Wasp
	struct Wasp
	{
		int _attackCount;				// 공격 횟수(스킬 사용을 위한)

		void Reset()
		{
			_attackCount = 0;
		}
	};
}