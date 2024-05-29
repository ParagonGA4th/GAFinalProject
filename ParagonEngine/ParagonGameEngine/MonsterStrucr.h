#pragma once

// 보스의 상태
enum class BossStatus
{
	ENTER,				// 조우 시 플레이어 마주치기 전
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적

	STAGGERED,			// 보스 무력화
	OFF_STAGGRED,
	DEAD,				// 사망

	PATTERN_READY,		// 공격 준비
	BASIC_ATTACK,		// 기본 공격
	CORE_ATTACK,		// 스킬 사용
	PATTERN_END			// 패턴이 끝난 경우
};