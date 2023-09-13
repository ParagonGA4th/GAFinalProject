#include "EngineMain.h"

namespace Pg::Engine
{
	EngineMain::EngineMain() :
		_inputSystem(Input::InputSystem::Instance())
	{

	}

	void EngineMain::Initialize(float width, float height)
	{
		_inputSystem->Initialize(width, height);
	}

	void EngineMain::Update()
	{
		_inputSystem->Update();
		/*using namespace Pg::API::Input;
				if (PgInput::GetKeyDown(eKeyCode::MouseLeft))
				{
					PG_TRACE("마우스 왼쪽 버튼 클릭");
				}
				if (PgInput::GetKey(eKeyCode::MouseRight))
				{
					std::string mouseX = std::to_string(PgInput::GetMouseX());
					std::string mouseY = std::to_string(PgInput::GetMouseY());
					std::string outString = "마우스 오른쪽 버튼 클릭 중 ";
					outString.append(mouseX).append(", ").append(mouseY);
					PG_TRACE(outString);
				}*/
		
	}

	void EngineMain::Finalize()
	{

	}

}
