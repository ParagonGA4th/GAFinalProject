#include "PgInput.h"
#include "../ParagonUtil/InputSystem.h"
#include <singleton-cpp/singleton.h>

//#ifdef _DEBUG
//#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
//#else
//#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
//#endif // _DEBUG

namespace Pg::API::Input
{
	//Pg::Engine::Input::InputSystem* PgInput::_inputSystem = nullptr;

	void PgInput::Initialize()
	{
		auto& tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		_inputSystem = &tInputSystem;
	}

	bool PgInput::GetKey(Pg::API::Input::eKeyCode keyCode)
	{
		return _inputSystem->GetKey(keyCode);
	}

	bool PgInput::GetKeyDown(Pg::API::Input::eKeyCode keyCode)
	{
		return _inputSystem->GetKeyDown(keyCode);
	}

	bool PgInput::GetKeyUp(Pg::API::Input::eKeyCode keyCode)
	{
		return _inputSystem->GetKeyUp(keyCode);
	}

	float PgInput::GetMouseX()
	{
		return _inputSystem->GetMouseX();
	}

	float PgInput::GetMouseY()
	{
		return _inputSystem->GetMouseY();
	}

	bool PgInput::IsMouseMoving()
	{
		return _inputSystem->IsMouseMoving();
	}

	float PgInput::GetMouseDX()
	{
		return _inputSystem->GetMouseDX();
	}

	float PgInput::GetMouseDY()
	{
		return _inputSystem->GetMouseDY();
	}

	void PgInput::HandleMessage(MSG& msg)
	{
		_inputSystem->HandleMessage(msg);
	}
	

}