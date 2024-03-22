#include "InputSystem.h"

#include "gainput/gainput.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonAPI/PgInput.h"

#include <windows.h>
#include <cassert>
#include <string>

//#ifdef _DEBUG
//#pragma comment (lib, "gainput-d.lib")
//#else
//#pragma comment (lib, "ginaput.lib")
//#endif // _DEBUG

//#ifdef _DEBUG
//#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
//#else
//#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
//#endif // _DEBUG

namespace  Pg::Util::Input
{
	InputSystem::InputSystem()
		:_manager(), _map(),
		_keyboardId(), _mouseId(), _padId(), _touchId()
	{
		_manager = new gainput::InputManager();
		_map = new gainput::InputMap(*_manager);

		_keyboardId = _manager->CreateDevice<gainput::InputDeviceKeyboard>();
		_mouseId = _manager->CreateDevice<gainput::InputDeviceMouse>();
		_padId = _manager->CreateDevice<gainput::InputDevicePad>();
		_touchId = _manager->CreateDevice<gainput::InputDeviceTouch>();

		//[DELETE] 인스턴스 두개인지 디버깅중.
		//std::string address = std::to_string((unsigned long long)(void**)this);

	}

	InputSystem::~InputSystem()
	{
		delete _map;
		delete _manager;
	}

	void InputSystem::Initialize(int screenwidth, int screenheight)
	{
		//PG_DEBUG("Input System Initialized.");
		OutputDebugString(L"Input System Initialized.");

		_manager->SetDisplaySize(screenwidth, screenheight);

		// 키와 액션을 맵핑
		MapKeys();

	}

	void InputSystem::Update()
	{
		_manager->Update();
	}

	void InputSystem::HandleMessage(MSG& msg)
	{
		_manager->HandleMessage(msg);
	}

	void InputSystem::MapKeys()
	{
		using namespace Pg::API::Input;

		assert(_map->MapBool(eKeyCode::MoveFront, _keyboardId, gainput::KeyW));
		assert(_map->MapBool(eKeyCode::MoveBack, _keyboardId, gainput::KeyS));
		assert(_map->MapBool(eKeyCode::MoveLeft, _keyboardId, gainput::KeyA));
		assert(_map->MapBool(eKeyCode::MoveRight, _keyboardId, gainput::KeyD));
		assert(_map->MapBool(eKeyCode::MoveUp, _keyboardId, gainput::KeyE));
		assert(_map->MapBool(eKeyCode::MoveDown, _keyboardId, gainput::KeyQ));
		assert(_map->MapBool(eKeyCode::KeyZ, _keyboardId, gainput::KeyZ));

		assert(_map->MapBool(eKeyCode::ButtonConfirm, _keyboardId, gainput::KeyReturn));
		assert(_map->MapBool(eKeyCode::ButtonConfirm, _padId, gainput::PadButtonA));
		assert(_map->MapBool(eKeyCode::ButtonConfirm, _touchId, gainput::Touch0Down));

		assert(_map->MapBool(eKeyCode::MouseLeft, _mouseId, gainput::MouseButtonLeft));
		assert(_map->MapBool(eKeyCode::MouseRight, _mouseId, gainput::MouseButtonRight));

		assert(_map->MapFloat(eKeyCode::MouseX, _mouseId, gainput::MouseAxisX));
		assert(_map->MapFloat(eKeyCode::MouseY, _mouseId, gainput::MouseAxisY));

		assert(_map->MapBool(eKeyCode::EditorOnOff, _keyboardId, gainput::KeyF1));
		assert(_map->MapBool(eKeyCode::DebugOnOff, _keyboardId, gainput::KeyF2));

		assert(_map->MapBool(eKeyCode::PrevRenderTarget, _keyboardId, gainput::Key9));
		assert(_map->MapBool(eKeyCode::NextRenderTarget, _keyboardId, gainput::Key0));

		assert(_map->MapBool(eKeyCode::ShiftL, _keyboardId, gainput::KeyShiftL));

		assert(_map->MapBool(eKeyCode::KeyLeft, _keyboardId, gainput::KeyLeft));
		assert(_map->MapBool(eKeyCode::KeyRight, _keyboardId, gainput::KeyRight));
		assert(_map->MapBool(eKeyCode::KeyUp, _keyboardId, gainput::KeyUp));
		assert(_map->MapBool(eKeyCode::KeyDown, _keyboardId, gainput::KeyDown));
		assert(_map->MapBool(eKeyCode::Space, _keyboardId, gainput::KeySpace));

		assert(_map->MapBool(eKeyCode::KeyB, _keyboardId, gainput::KeyB));
		assert(_map->MapBool(eKeyCode::KeyN, _keyboardId, gainput::KeyN));
		assert(_map->MapBool(eKeyCode::KeyM, _keyboardId, gainput::KeyM));

		gainput::SimultaneouslyDownGesture* sdg = _manager->CreateAndGetDevice<gainput::SimultaneouslyDownGesture>();
		sdg->AddButton(_keyboardId, gainput::KeyCtrlL);
		sdg->AddButton(_keyboardId, gainput::KeyS);
		_map->MapBool(eKeyCode::Save, sdg->GetDeviceId(), gainput::SimultaneouslyDownTriggered);
	}

	bool InputSystem::GetKey(Pg::API::Input::eKeyCode keyCode)
	{
		return _map->GetBool(keyCode);
	}

	bool InputSystem::GetKeyDown(Pg::API::Input::eKeyCode keyCode)
	{
		return _map->GetBoolIsNew(keyCode);
	}

	bool InputSystem::GetKeyUp(Pg::API::Input::eKeyCode keyCode)
	{
		return _map->GetBoolWasDown(keyCode);
	}

	float InputSystem::GetMouseX()
	{
		return _map->GetFloat(Pg::API::Input::eKeyCode::MouseX);
	}

	float InputSystem::GetMouseY()
	{
		return _map->GetFloat(Pg::API::Input::eKeyCode::MouseY);
	}

	bool InputSystem::IsMouseMoving()
	{
		if (_map->GetFloatDelta(Pg::API::Input::eKeyCode::MouseX) != 0.0f || _map->GetFloatDelta(Pg::API::Input::eKeyCode::MouseY) != 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	float InputSystem::GetMouseDX()
	{
		return _map->GetFloatDelta(Pg::API::Input::eKeyCode::MouseX);
	}

	float InputSystem::GetMouseDY()
	{
		return _map->GetFloatDelta(Pg::API::Input::eKeyCode::MouseY);
	}
}