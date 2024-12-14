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

		bool _isDownInit;				// ���� ����ȭ ����
		bool _isDown;					// ������ ����ȭ �Ǿ�����
		bool _isDownEnd;				// ���� ����ȭ ����

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

		bool _isSkill;					// ��ų ��� ����
		bool _isDown;					// ��ų ��� ����

		bool _isDash;					// �� �뽬

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
		bool _isDash;					// �� �뽬

		void Reset()
		{
			_isDash = false;
		}
	};

	/// Trent
	struct Stub
	{
		Pg::Data::StubState _stubState = Pg::Data::StubState::IDLE; // �׷��ͱ��� ���� ����(�ൿ����)
	
		void Reset()
		{
			_stubState = Pg::Data::StubState::IDLE;
		}
	};

	/// Mimic
	struct Mimic
	{
		bool _isTouched;				// �÷��̾ �̹Ͱ� ��ȣ�ۿ� �ߴ��� 

		void Reset()
		{
			_isTouched = false;
		}
	};

	/// Wasp
	struct Wasp
	{
		int _attackCount;				// ���� Ƚ��(��ų ����� ����)

		void Reset()
		{
			_attackCount = 0;
		}
	};
}