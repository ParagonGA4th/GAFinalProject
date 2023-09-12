#include "InputSystem.h"

#include "gainput/gainput.h"
#include "../ParagonUtil/Log.h"

#include <windows.h>
#include <cassert>

#ifdef _DEBUG
#pragma comment (lib, "gainput-d.lib")
#else
#pragma comment (lib, "ginaput.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace  Pg::Engine::Input
{
	InputSystem::InputSystem()
		:_manager(), _map(_manager),
		keyboardId(), mouseId(), padId(), touchId()
	{
		
	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::Initialize(int screenwidth, int screenheight)
	{
		//PG_DEBUG("click");
		
		_manager.SetDisplaySize(screenwidth, screenheight);

		keyboardId = _manager.CreateDevice<gainput::InputDeviceKeyboard>();
		mouseId = _manager.CreateDevice<gainput::InputDeviceMouse>();
		padId = _manager.CreateDevice<gainput::InputDevicePad>();
		touchId = _manager.CreateDevice<gainput::InputDeviceTouch>();

		_map.MapBool(ButtonConfirm, keyboardId, gainput::KeyReturn);
		_map.MapBool(ButtonConfirm, mouseId, gainput::MouseButtonLeft);
		_map.MapBool(ButtonConfirm, padId, gainput::PadButtonA);
		_map.MapBool(ButtonConfirm, touchId, gainput::Touch0Down);

		_map.MapFloat(MouseX, mouseId, gainput::MouseAxisX);
		_map.MapFloat(MouseY, mouseId, gainput::MouseAxisY);
	}


	void InputSystem::Update()
	{
		_manager.Update();

		if (_map.GetBoolWasDown(ButtonConfirm))
		{
			assert(false);
			OutputDebugString(L"game injaewon");
		}
			
	}
}

