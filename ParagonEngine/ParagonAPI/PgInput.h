#pragma once
#include "KeyCodeType.h"
#include "gainput/gainput.h"

/// <summary>
/// API 딴에서 Input을 갖다 쓸 수 있게 하는 클래스.
/// </summary>


namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::API
{
	class APIMain;
}

namespace Pg::API::Input
{
	class PgInput
	{
		friend class Pg::API::APIMain;
	public:
		static bool GetKey(Pg::API::Input::eKeyCode keyCode);
		static bool GetKeyDown(Pg::API::Input::eKeyCode keyCode);
		static bool GetKeyUp(Pg::API::Input::eKeyCode keyCode);

		static float GetMouseX();
		static float GetMouseY();

		static float GetMouseDX();
		static float GetMouseDY();

		static bool IsMouseMoving();

		static void HandleMessage(MSG& msg);

	private:
		static void Initialize();

	private:
		static Pg::Engine::Input::InputSystem* _inputSystem;
	};


}
