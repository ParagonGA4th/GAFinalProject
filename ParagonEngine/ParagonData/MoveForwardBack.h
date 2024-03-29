#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include <cstdlib>

/// <summary>
/// GameObject 내부 스크립트가 Graphics랑 연동해서 작동하는지 알아보기 위해.
/// </summary>

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}
namespace Pg::Data
{
	class MoveForwardBack : public Component
	{
	public:
		MoveForwardBack(GameObject* obj);

		virtual void Update() override;

	private:
		DynamicCollider* _col;

		Pg::Util::Input::InputSystem* _input;

	};

}

