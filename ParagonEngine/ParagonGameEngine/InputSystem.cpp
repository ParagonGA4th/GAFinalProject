#include "InputSystem.h"

#include "gainput/gainput.h"
#include "../ParagonUtil/Log.h"

#include <windows.h>
#include <cassert>
#include <string>

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
		:_manager(), _map(),
		_keyboardId(), _mouseId(), _padId(), _touchId()
	{
		_manager = new gainput::InputManager();

		_keyboardId = _manager->CreateDevice<gainput::InputDeviceKeyboard>();
		_mouseId = _manager->CreateDevice<gainput::InputDeviceMouse>();
		_padId = _manager->CreateDevice<gainput::InputDevicePad>();
		_touchId = _manager->CreateDevice<gainput::InputDeviceTouch>();

		_map = new gainput::InputMap(*_manager);
	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::Initialize(int screenwidth, int screenheight)
	{
		//PG_DEBUG("Input System Initialized.");
		OutputDebugString(L"Input System Initialized.");
		
		_manager->SetDisplaySize(screenwidth, screenheight);

		_map->MapBool(ButtonConfirm, _keyboardId, gainput::KeyReturn);
		_map->MapBool(ButtonConfirm, _mouseId, gainput::MouseButtonLeft);
		_map->MapBool(ButtonConfirm, _padId, gainput::PadButtonA);
		_map->MapBool(ButtonConfirm, _touchId, gainput::Touch0Down);

		_map->MapFloat(MouseX, _mouseId, gainput::MouseAxisX);
		_map->MapFloat(MouseY, _mouseId, gainput::MouseAxisY);

	}

	void InputSystem::Update()
	{
		_manager->Update();

		if (_map->GetBoolWasDown(ButtonConfirm))
		{
			OutputDebugString(L"Confirm Button Clicked.");
			PG_TRACE(" ");
		}

		if (_map->GetFloatDelta(MouseX) != 0.0f || _map->GetFloatDelta(MouseY) != 0.0f)
		{
			std::wstring output = L"";
			output += L"Mouse Moved. X: ";
			output += std::to_wstring(_map->GetFloat(MouseX));
			output += L" Y: ";
			output += std::to_wstring(_map->GetFloat(MouseY));
			output += L"\n";

			OutputDebugString(output.c_str());
		}

	}

	void InputSystem::HandleMessage(MSG& msg)
	{
		_manager->HandleMessage(msg);
	}

}

