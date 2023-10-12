#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// GameObject 내부 스크립트가 Graphics랑 연동해서 작동하는지 알아보기 위해.
/// </summary>

namespace Pg::Data
{
	class GameObject;
}
namespace Pg::Data
{
	class MoveForwardBack : public Component
	{
	public:
		MoveForwardBack(GameObject* obj);
		virtual void Update() override;

	private:
		float tMoveVar = 0.0f;
	};

}

