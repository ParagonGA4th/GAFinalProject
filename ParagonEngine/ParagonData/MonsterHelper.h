#pragma once
#include "Component.h"

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
			_isPlayerDetected(false), _isPlayerinHitSpace(false), _isDash(false), _isChase(false),
			_isAnimationEnd(false) {};

		virtual void OnAnimationEnd()
		{
			_isAnimationEnd = true;
		};

		/// Public
		bool _isPlayerDetected;			// 플레이어가 시야 안에 있는지
		bool _isPlayerinHitSpace;		// 플레이어가 공격 범위 안에 있는지
		bool _isChase;					// 몬스터가 플레이어를 쫒고 있는지
		bool _isHit;					// 몬스터가 공격 받았는지

		/// Golem
		bool _isDash;					// 몬스터가 대쉬를 했는지(골렘 전용)

		/// Animation
		bool _isAnimationEnd;			// 애니매이션이 끝났는지
	};
}

