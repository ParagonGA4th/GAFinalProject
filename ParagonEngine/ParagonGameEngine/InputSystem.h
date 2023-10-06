///
/// Gainput Library를 사용한 InputSystem 구현
/// 
/// 2023. 09. 11. 김정식

#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonAPI/KeyCodeType.h"
#include "gainput/gainput.h"

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::Engine::Input
{
	class InputSystem
	{
		friend class Pg::API::Input::PgInput;
		//friend class Singleton<InputSystem>;
	
	public:
		PARAGON_ENGINE_DLL InputSystem();
		PARAGON_ENGINE_DLL ~InputSystem();

	public:
		PARAGON_ENGINE_DLL void Initialize(int screenwidth, int screenheight);
		PARAGON_ENGINE_DLL void Update();

	public:
		PARAGON_ENGINE_DLL void HandleMessage(MSG& msg);
	
	private:
		void MapKeys();

	public:
		PARAGON_ENGINE_DLL bool GetKey(Pg::API::Input::eKeyCode keyCode);
		PARAGON_ENGINE_DLL bool GetKeyDown(Pg::API::Input::eKeyCode keyCode);
		PARAGON_ENGINE_DLL bool GetKeyUp(Pg::API::Input::eKeyCode keyCode);

		PARAGON_ENGINE_DLL float GetMouseX();
		PARAGON_ENGINE_DLL float GetMouseY();
		
		PARAGON_ENGINE_DLL float GetMouseDX();
		PARAGON_ENGINE_DLL float GetMouseDY();

		PARAGON_ENGINE_DLL bool IsMouseMoving();

	private:
		gainput::InputManager* _manager;
		gainput::InputMap* _map;

		gainput::DeviceId _keyboardId;
		gainput::DeviceId _mouseId;
		gainput::DeviceId _padId;
		gainput::DeviceId _touchId;
	};
}

