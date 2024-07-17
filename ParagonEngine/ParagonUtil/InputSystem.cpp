#include "InputSystem.h"
#include "CustomAssert.h"

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

namespace Pg::Util::Input
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

		CustomAssert(_map->MapBool(eKeyCode::Esc, _keyboardId, gainput::KeyEscape));

		CustomAssert(_map->MapBool(eKeyCode::MoveFront, _keyboardId, gainput::KeyW));
		CustomAssert(_map->MapBool(eKeyCode::MoveBack, _keyboardId, gainput::KeyS));
		CustomAssert(_map->MapBool(eKeyCode::MoveLeft, _keyboardId, gainput::KeyA));
		CustomAssert(_map->MapBool(eKeyCode::MoveRight, _keyboardId, gainput::KeyD));
		CustomAssert(_map->MapBool(eKeyCode::KeyE, _keyboardId, gainput::KeyE));
		CustomAssert(_map->MapBool(eKeyCode::KeyQ, _keyboardId, gainput::KeyQ));
		CustomAssert(_map->MapBool(eKeyCode::KeyZ, _keyboardId, gainput::KeyZ));
		CustomAssert(_map->MapBool(eKeyCode::KeyF, _keyboardId, gainput::KeyF));
		CustomAssert(_map->MapBool(eKeyCode::CtrlL, _keyboardId, gainput::KeyCtrlL));

		CustomAssert(_map->MapBool(eKeyCode::ButtonConfirm, _keyboardId, gainput::KeyReturn));
		CustomAssert(_map->MapBool(eKeyCode::ButtonConfirm, _padId, gainput::PadButtonA));
		CustomAssert(_map->MapBool(eKeyCode::ButtonConfirm, _touchId, gainput::Touch0Down));

		CustomAssert(_map->MapBool(eKeyCode::MouseLeft, _mouseId, gainput::MouseButtonLeft));
		CustomAssert(_map->MapBool(eKeyCode::MouseRight, _mouseId, gainput::MouseButtonRight));

		CustomAssert(_map->MapFloat(eKeyCode::MouseX, _mouseId, gainput::MouseAxisX));
		CustomAssert(_map->MapFloat(eKeyCode::MouseY, _mouseId, gainput::MouseAxisY));

		CustomAssert(_map->MapBool(eKeyCode::EditorOnOff, _keyboardId, gainput::KeyF1));
		CustomAssert(_map->MapBool(eKeyCode::DebugOnOff, _keyboardId, gainput::KeyF2));

		CustomAssert(_map->MapBool(eKeyCode::PrevRenderTarget, _keyboardId, gainput::Key9));
		CustomAssert(_map->MapBool(eKeyCode::NextRenderTarget, _keyboardId, gainput::Key0));

		CustomAssert(_map->MapBool(eKeyCode::ShiftL, _keyboardId, gainput::KeyShiftL));
		CustomAssert(_map->MapBool(eKeyCode::ShiftR, _keyboardId, gainput::KeyShiftR));

		CustomAssert(_map->MapBool(eKeyCode::KeyLeft, _keyboardId, gainput::KeyLeft));
		CustomAssert(_map->MapBool(eKeyCode::KeyRight, _keyboardId, gainput::KeyRight));
		CustomAssert(_map->MapBool(eKeyCode::KeyUp, _keyboardId, gainput::KeyUp));
		CustomAssert(_map->MapBool(eKeyCode::KeyDown, _keyboardId, gainput::KeyDown));
		CustomAssert(_map->MapBool(eKeyCode::Space, _keyboardId, gainput::KeySpace));

		CustomAssert(_map->MapBool(eKeyCode::KeyB, _keyboardId, gainput::KeyB));
		CustomAssert(_map->MapBool(eKeyCode::KeyN, _keyboardId, gainput::KeyN));
		CustomAssert(_map->MapBool(eKeyCode::KeyM, _keyboardId, gainput::KeyM));
		CustomAssert(_map->MapBool(eKeyCode::KeyF, _keyboardId, gainput::KeyF));

		CustomAssert(_map->MapBool(eKeyCode::KeyX, _keyboardId, gainput::KeyX));
		CustomAssert(_map->MapBool(eKeyCode::KeyZ, _keyboardId, gainput::KeyZ));

		CustomAssert(_map->MapBool(eKeyCode::KeyZ, _keyboardId, gainput::KeyZ));

		CustomAssert(_map->MapBool(eKeyCode::KeyBackSlash, _keyboardId, gainput::KeyBackslash));

		//숫자들 싹 다 매핑해놓았음.
		CustomAssert(_map->MapBool(eKeyCode::KeyNum1, _keyboardId, gainput::Key1));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum2, _keyboardId, gainput::Key2));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum3, _keyboardId, gainput::Key3));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum4, _keyboardId, gainput::Key4));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum5, _keyboardId, gainput::Key5));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum6, _keyboardId, gainput::Key6));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum7, _keyboardId, gainput::Key7));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum8, _keyboardId, gainput::Key8));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum9, _keyboardId, gainput::Key9));
		CustomAssert(_map->MapBool(eKeyCode::KeyNum0, _keyboardId, gainput::Key0));

		CustomAssert(_map->MapBool(eKeyCode::KeyR, _keyboardId, gainput::KeyR));

		//gainput::SimultaneouslyDownGesture* sdg = _manager->CreateAndGetDevice<gainput::SimultaneouslyDownGesture>();
		//sdg->AddButton(_keyboardId, gainput::KeyCtrlL);
		//sdg->AddButton(_keyboardId, gainput::KeyS);
		//_map->MapBool(eKeyCode::Save, sdg->GetDeviceId(), gainput::SimultaneouslyDownTriggered);
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
		if (_editorMouseX != -10.f) return _editorMouseX;
		else return _map->GetFloat(Pg::API::Input::eKeyCode::MouseX);
	}

	float InputSystem::GetMouseY()
	{
		if (_editorMouseY != -10.f) return _editorMouseY;
		else return _map->GetFloat(Pg::API::Input::eKeyCode::MouseY);
	}

	void InputSystem::SetEditorMouseX(float x)
	{
		_editorMouseX = x;
	}

	void InputSystem::SetEditorMouseY(float y)
	{
		_editorMouseY = y;
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