///
/// Gainput Library를 사용한 InputSystem 구현
/// 
/// 2023. 09. 11. 김정식

#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonCore/Singleton.h"

#include "gainput/gainput.h"

#include "../ParagonAPI/PgInput.h"

namespace Pg::Engine::Input
{

	class InputSystem : public Singleton<InputSystem>
	{
		friend class Pg::API::Input::PgInput;
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

