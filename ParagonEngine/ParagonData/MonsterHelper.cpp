#include "MonsterHelper.h"

Pg::Data::MonsterHelper::MonsterHelper(GameObject* obj)
	: Component(obj),
	_isPlayerDetected(false), _isPlayerinHitSpace(false),
	_isDistanceClose(false), _isChase(false), _isHit(false), _isDead(false),
	_isAnimationEnd(false), _isAnimChange(false), _isDeadDelay(false)
{
	BossInit();
	GolemBossInit();
	GolemInit();
	StubInit();
	MimicInit();
	WaspInit();
}

void Pg::Data::MonsterHelper::OnAnimationEnd(const std::string& justEndedAnimation)
{
	_isAnimationEnd = true;
}

void Pg::Data::MonsterHelper::Reset()
{
	_isPlayerDetected = false; 
	_isPlayerinHitSpace = false;
	_isDistanceClose = false;
	_isChase = false; 
	_isHit = false;
	_isDead = false;
	_isAnimationEnd = false;
	_isAnimChange = false; 
	_isDeadDelay = false;

	_bossFlag.Reset();
	_bGolemFlag.Reset();
	_mGolemFlag.Reset();
	_stubFlag.Reset();
	_mimicFlag.Reset();
	_waspFlag.Reset();
}

void Pg::Data::MonsterHelper::BossInit()
{
	_bossFlag._isDownInit = false;
	_bossFlag._isDown = false;
	_bossFlag._isDownEnd = false;

	_bossFlag._bossState = Pg::Data::BossState::IDLE;
	_bossFlag._bossPase = Pg::Data::BossPase::PASE_1;

	_bossFlag._bossStateListByEnum[Pg::Data::BossState::IDLE] = "Idle";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::CAST] = "Cast";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::DASH] = "Dash";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_1] = "BAttack_1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_2] = "BAttack_2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_3] = "BAttack_3";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK] = "SFeatherAttack";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK_PREPARE] = "SFeatherAttackPre";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK_END] = "SFeatherAttackEnd";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_1] = "SFlyAttack1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1] = "SFlyPre1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_2] = "SFlyAttack2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2] = "SFlyPre2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_3] = "SFlyAttack3";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3] = "SFlyPre3";
}

void Pg::Data::MonsterHelper::GolemBossInit()
{
	_bGolemFlag._isDash = false;
	_bGolemFlag._isDown = false;

	_bGolemFlag._bossState = Pg::Data::GolemBossState::IDLE;
	_bGolemFlag._bossPase = Pg::Data::BossPase::PASE_1;

	_bGolemFlag._bossStateListByEnum[Pg::Data::GolemBossState::IDLE] = "Idle";
	_bGolemFlag._bossStateListByEnum[Pg::Data::GolemBossState::BASIC_ATTACK] = "BAttack";
	_bGolemFlag._bossStateListByEnum[Pg::Data::GolemBossState::SKILL_ATTACK_1] = "SAttack_1";
	_bGolemFlag._bossStateListByEnum[Pg::Data::GolemBossState::SKILL_ATTACK_2] = "SAttack_2";
	_bGolemFlag._bossStateListByEnum[Pg::Data::GolemBossState::SKILL_DASH_ATTACK] = "SpinDashAttack";
}

void Pg::Data::MonsterHelper::GolemInit()
{
	_mGolemFlag._isDash = false;
}

void Pg::Data::MonsterHelper::StubInit()
{
	_stubFlag._stubState = Pg::Data::StubState::IDLE;
}

void Pg::Data::MonsterHelper::MimicInit()
{
	_mimicFlag._isTouched = false;
}

void Pg::Data::MonsterHelper::WaspInit()
{
	_waspFlag._attackCount = 0;
}