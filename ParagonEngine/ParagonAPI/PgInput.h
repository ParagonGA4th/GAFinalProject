#pragma once
#include "KeyCodeType.h"
#include "gainput/gainput.h"

/// <summary>
/// API 딴에서 Input을 갖다 쓸 수 있게 하는 클래스.
/// </summary>


namespace Pg::Util
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
		bool GetKey(Pg::API::Input::eKeyCode keyCode);
		bool GetKeyDown(Pg::API::Input::eKeyCode keyCode);
		bool GetKeyUp(Pg::API::Input::eKeyCode keyCode);

		float GetMouseX();
		float GetMouseY();

		float GetMouseDX();
		float GetMouseDY();

		bool IsMouseMoving();

		void HandleMessage(MSG& msg);

	private:
		void Initialize();

	private:
		Pg::Util::Input::InputSystem* _inputSystem;
	};


}
