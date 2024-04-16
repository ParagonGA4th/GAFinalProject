#pragma once
#include "../ParagonData/Component.h"


namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>
class AISeight : public Pg::Data::Component
{
public:
	virtual void Start() override;
	virtual void Update() override;
};

