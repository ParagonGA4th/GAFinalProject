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
			_isPlayerDetected(false), _isPlayerinHitSpace(false), _isDistanceClose(false),
			_isAnimationEnd(false) {};

		virtual void OnAnimationEnd()
		{
			_isAnimationEnd = true;
		};

		bool _isPlayerDetected;
		bool _isPlayerinHitSpace;
		bool _isDistanceClose;
		bool _isAnimationEnd;
		bool _isHit;
	};
}

