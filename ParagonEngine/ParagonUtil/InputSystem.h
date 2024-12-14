///
/// Gainput Library를 사용한 InputSystem 구현
/// 
/// 2023. 09. 11. 김정식

#pragma once

#include "UtilDLLExporter.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonAPI/KeyCodeType.h"
#include <gainput/gainput.h>

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::Util::Input
{
	class InputSystem
	{
		friend class Pg::API::Input::PgInput;
		//friend class Singleton<InputSystem>;

	public:
		PARAGON_UTIL_DLL InputSystem();
		PARAGON_UTIL_DLL ~InputSystem();

	public:
		PARAGON_UTIL_DLL void Initialize(int screenwidth, int screenheight);
		PARAGON_UTIL_DLL void Update();

	public:
		PARAGON_UTIL_DLL void HandleMessage(MSG& msg);

	private:
		void MapKeys();

	public:
		PARAGON_UTIL_DLL bool GetKey(Pg::API::Input::eKeyCode keyCode);
		PARAGON_UTIL_DLL bool GetKeyDown(Pg::API::Input::eKeyCode keyCode);
		PARAGON_UTIL_DLL bool GetKeyUp(Pg::API::Input::eKeyCode keyCode);

		PARAGON_UTIL_DLL float GetMouseX();
		PARAGON_UTIL_DLL float GetMouseY();

		// editor ui에서 쓰이는 함수
		PARAGON_UTIL_DLL void SetEditorMouseX(float x);
		// editor ui에서 쓰이는 함수
		PARAGON_UTIL_DLL void SetEditorMouseY(float y);

		PARAGON_UTIL_DLL float GetMouseDX();
		PARAGON_UTIL_DLL float GetMouseDY();

		PARAGON_UTIL_DLL bool IsMouseMoving();

	private:
		gainput::InputManager* _manager;
		gainput::InputMap* _map;

		gainput::DeviceId _keyboardId;
		gainput::DeviceId _mouseId;
		gainput::DeviceId _padId;
		gainput::DeviceId _touchId;

		float _editorMouseX;
		float _editorMouseY;
	};
}