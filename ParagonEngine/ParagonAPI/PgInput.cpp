#include "PgInput.h"

#include "../ParagonGameEngine/InputSystem.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::Input
{
	bool PgInput::GetKey(Pg::API::Input::eKeyCode keyCode)
	{
		return Pg::Engine::Input::InputSystem::Instance()->GetKey(keyCode);
	}

	bool PgInput::GetKeyDown(Pg::API::Input::eKeyCode keyCode)
	{
		return Pg::Engine::Input::InputSystem::Instance()->GetKeyDown(keyCode);
	}

	bool PgInput::GetKeyUp(Pg::API::Input::eKeyCode keyCode)
	{
		return Pg::Engine::Input::InputSystem::Instance()->GetKeyUp(keyCode);
	}

	float PgInput::GetMouseX()
	{
		return Pg::Engine::Input::InputSystem::Instance()->GetMouseX();
	}

	float PgInput::GetMouseY()
	{
		return Pg::Engine::Input::InputSystem::Instance()->GetMouseY();
	}

	bool PgInput::IsMouseMoving()
	{
		return Pg::Engine::Input::InputSystem::Instance()->IsMouseMoving();
	}

}