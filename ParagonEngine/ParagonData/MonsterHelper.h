#pragma once
#include "Component.h"
#include "MonsterState.h"
#include <unordered_map>

/// <summary>
/// 24.05.20 최민서
/// script에 있는 변수 값을 가져오기 위한 helperClass
/// BTNode에 쓰일 예정
/// </summary>

namespace Pg::Data
{
	class MonsterHelper : public Component
	{
	public:
		// 임시 기본생성자
		MonsterHelper() = default;
		MonsterHelper(GameObject* obj) : Component(obj),
			_isPlayerDetected(false), _isPlayerinHitSpace(false),
			_isDistanceClose(false), _isChase(false), _isHit(false), _isDead(false),
			_isPase_1(false), _isPase_2(false), _isPase_3(false), _isDown(false), _isDistanceFar(false),
			_isDash(false),
			_isTouched(false),
			_isAnimationEnd(false), _isAnimChange(false), _isDeadDelay(false)
		{
			_bossStateListByString["Idle"] = Pg::Data::BossState::IDLE;
			_bossStateListByString["BAttack_1"] = Pg::Data::BossState::BASIC_ATTACK_1;
			_bossStateListByString["BAttack_2"] = Pg::Data::BossState::BASIC_ATTACK_2;
			_bossStateListByString["BAttack_3"] = Pg::Data::BossState::BASIC_ATTACK_3;
			_bossStateListByString["SFeatherAttack"] = Pg::Data::BossState::SKILL_FEATHER_ATTACK;
			_bossStateListByString["SFlyAttack1"] = Pg::Data::BossState::SKILL_FLY_ATTACK_1;
			_bossStateListByString["SFlyAttackPre1"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
			_bossStateListByString["SFlyAttack2"] = Pg::Data::BossState::SKILL_FLY_ATTACK_2;
			_bossStateListByString["SFlyAttackPre2"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;
			_bossStateListByString["SFlyAttack3"] = Pg::Data::BossState::SKILL_FLY_ATTACK_3;
			_bossStateListByString["SFlyAttackPre3"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;

			_bossStateListByEnum[Pg::Data::BossState::IDLE] = "Idle";
			_bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_1] = "BAttack_1";
			_bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_2] = "BAttack_2";
			_bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_3] = "BAttack_3";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK] = "SFeatherAttack";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_1] = "SFlyAttack1";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1] = "SFlyAttackPre1";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_2] = "SFlyAttack2";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2] = "SFlyAttackPre2";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_3] = "SFlyAttack3";
			_bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3] = "SFlyAttackPre3";
		};

		virtual void OnAnimationEnd()
		{
			_isAnimationEnd = true;
		};

		/// Public
		bool _isPlayerDetected;			// 플레이어가 시야 안에 있는지
		bool _isPlayerinHitSpace;		// 플레이어가 공격 범위 안에 있는지
		bool _isDistanceClose;			// 플레이어와 거리가 가까운지(스킬을 쓰기 위한 거리)
		bool _isChase;					// 몬스터가 플레이어를 쫒고 있는지
		bool _isHit;					// 몬스터가 공격 받았는지
		bool _isDead;					// 몬스터가 죽었는지

		/// Boss
		Pg::Data::BossState _bossState = Pg::Data::BossState::IDLE;
		Pg::Data::BossPase _bossPase = Pg::Data::BossPase::PASE_1;
		std::unordered_map<std::string, Pg::Data::BossState> _bossStateListByString;
		std::unordered_map<Pg::Data::BossState, std::string> _bossStateListByEnum;
		bool _isPase_1;					// 보스의 1페이즈
		bool _isPase_2;					// 보스의 1페이즈
		bool _isPase_3;					// 보스의 1페이즈
		bool _isDown;					// 보스가 무력화 되었는지
		bool _isDistanceFar;			// 플레이어와 보스의 거리가 먼 지

		/// Golem
		bool _isDash;					// 몬스터가 대쉬를 했는지(with Boss)

		/// Trent
		Pg::Data::TrentState _trentState = Pg::Data::TrentState::IDLE; // 그루터기의 현재 상태(행동패턴)

		/// Mimic
		bool _isTouched;				// 플레이어가 미믹과 상호작용 했는지 

		/// Animation
		bool _isAnimationEnd;			// 애니매이션이 끝났는지
		bool _isAnimChange;				// 애니매이션이 바뀌었는지
		bool _isDeadDelay;				// 몬스터가 죽은 후 딜레이
	};
}

