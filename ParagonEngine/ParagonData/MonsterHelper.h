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
		MonsterHelper(GameObject* obj) : Component(obj), _isPlayerDetected(false), _isPlayer(false) {};

		bool _isPlayerDetected;
		bool _isPlayer;
	};
}

