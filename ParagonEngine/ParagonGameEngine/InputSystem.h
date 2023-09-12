///
/// Gainput Library를 사용한 InputSystem 구현
/// 
/// 2023. 09. 11. 김정식

#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"

#include "gainput/gainput.h"

namespace Pg::Engine::Input
{
	enum eInput
	{
		ButtonMenu,
		ButtonConfirm,
		MouseX,
		MouseY
	};

	class InputSystem
	{
	public:
		PARAGON_ENGINE_DLL InputSystem();
		PARAGON_ENGINE_DLL ~InputSystem();

	public:
		PARAGON_ENGINE_DLL void Initialize(int screenwidth, int screenheight);
		PARAGON_ENGINE_DLL void Update();
	
	public:
		gainput::InputManager _manager;
		gainput::InputMap _map;

		gainput::DeviceId keyboardId;
		gainput::DeviceId mouseId;
		gainput::DeviceId padId;
		gainput::DeviceId touchId;
	};
}

