#pragma once

// 보스의 상태
enum class BossStatus
{
	ENTER,				// 조우 시 플레이어 마주치기 전
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적
	RUSH,				// 돌진

	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
	SKILL_ATTACK,		// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
	PATTERN_END			// 패턴이 끝난 경우
};