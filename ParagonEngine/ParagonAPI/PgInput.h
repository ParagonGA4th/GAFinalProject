#pragma once
#include "KeyCodeType.h"

namespace Pg::API::Input
{
	class PgInput
	{
	public:
		static bool GetKey(Pg::API::Input::eKeyCode keyCode);
		static bool GetKeyDown(Pg::API::Input::eKeyCode keyCode);
		static bool GetKeyUp(Pg::API::Input::eKeyCode keyCode);

		static float GetMouseX();
		static float GetMouseY();

		static bool IsMouseMoving();

	};


}
