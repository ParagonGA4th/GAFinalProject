#pragma once
#include "Component.h"
#include "MonsterFlag.h"


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
		MonsterHelper(GameObject* obj);

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;
		void Reset();

	private:
		void BossInit();
		void GolemBossInit();
		void GolemInit();
		void StubInit();
		void MimicInit();
		void WaspInit();

	public:
		/// Public
		bool _isPlayerDetected;			// 플레이어가 시야 안에 있는지
		bool _isPlayerinHitSpace;		// 플레이어가 공격 범위 안에 있는지
		bool _isDistanceClose;			// 플레이어와 거리가 가까운지(스킬을 쓰기 위한 거리)
		bool _isChase;					// 몬스터가 플레이어를 쫒고 있는지
		bool _isHit;					// 몬스터가 공격 받았는지
		bool _isDead;					// 몬스터가 죽었는지

		/// Animation
		bool _isAnimationEnd;			// 애니매이션이 끝났는지
		bool _isAnimChange;				// 애니매이션이 바뀌었는지
		bool _isDeadDelay;				// 몬스터가 죽은 후 딜레이

		/// Monsters
		Pg::Data::MonsterState _state;	// 패턴이 없는 몬스터들의 state

		Pg::Data::Boss _bossFlag;
		Pg::Data::GolemBoss _bGolemFlag;
		Pg::Data::MiniGolem _mGolemFlag;
		Pg::Data::Stub _stubFlag;
		Pg::Data::Mimic _mimicFlag;
		Pg::Data::Wasp _waspFlag;
	};
}

