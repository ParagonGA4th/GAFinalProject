#include "MonsterHelper.h"

Pg::Data::MonsterHelper::MonsterHelper(GameObject* obj)
	: Component(obj),
	_isPlayerDetected(false), _isPlayerinHitSpace(false),
	_isDistanceClose(false), _isChase(false), _isHit(false), _isDead(false),
	_isAnimationEnd(false), _isAnimChange(false), _isDeadDelay(false)
{
	BossInit();
	GolemInit();
	StubInit();
	MimicInit();
}
void Pg::Data::MonsterHelper::OnAnimationEnd(const std::string& justEndedAnimation)
{
	_isAnimationEnd = true;
}
void Pg::Data::MonsterHelper::BossInit()
{
	_bossFlag._isPase_1 = true;
	_bossFlag._isPase_2 = false;
	_bossFlag._isPase_3 = false;

	_bossFlag._isDash = false;
	_bossFlag._isDown = false;

	_bossFlag._bossState = Pg::Data::BossState::IDLE;
	_bossFlag._bossPase = Pg::Data::BossPase::PASE_1;

	_bossFlag._bossStateListByString["Idle"] = Pg::Data::BossState::IDLE;
	_bossFlag._bossStateListByString["BAttack_1"] = Pg::Data::BossState::BASIC_ATTACK_1;
	_bossFlag._bossStateListByString["BAttack_2"] = Pg::Data::BossState::BASIC_ATTACK_2;
	_bossFlag._bossStateListByString["BAttack_3"] = Pg::Data::BossState::BASIC_ATTACK_3;
	_bossFlag._bossStateListByString["SFeatherAttack"] = Pg::Data::BossState::SKILL_FEATHER_ATTACK;
	_bossFlag._bossStateListByString["SFlyAttack1"] = Pg::Data::BossState::SKILL_FLY_ATTACK_1;
	_bossFlag._bossStateListByString["SFlyAttackPre1"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
	_bossFlag._bossStateListByString["SFlyAttack2"] = Pg::Data::BossState::SKILL_FLY_ATTACK_2;
	_bossFlag._bossStateListByString["SFlyAttackPre2"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;
	_bossFlag._bossStateListByString["SFlyAttack3"] = Pg::Data::BossState::SKILL_FLY_ATTACK_3;
	_bossFlag._bossStateListByString["SFlyAttackPre3"] = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;

	_bossFlag._bossStateListByEnum[Pg::Data::BossState::IDLE] = "Idle";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_1] = "BAttack_1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_2] = "BAttack_2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_3] = "BAttack_3";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK] = "SFeatherAttack";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_1] = "SFlyAttack1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1] = "SFlyAttackPre1";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_2] = "SFlyAttack2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2] = "SFlyAttackPre2";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_3] = "SFlyAttack3";
	_bossFlag._bossStateListByEnum[Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3] = "SFlyAttackPre3";
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
