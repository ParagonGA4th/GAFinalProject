#include "EngineMain.h"
#include "InputSystem.h"

#include "../ParagonUtil/Log.h"
#include "../ParagonAPI/KeyCodeType.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

namespace Pg::Engine
{
	EngineMain::EngineMain()
	{
		auto& tInputSystem = singleton<Input::InputSystem>();
		_inputSystem = &tInputSystem;
	}

	void EngineMain::Initialize(float width, float height)
	{
		_inputSystem->Initialize(width, height);
	}

	void EngineMain::Update()
	{
		_inputSystem->Update();

		static bool tTest = false;
		if (!tTest)
		{
			PG_TRACE("DKDKK");
			tTest = true;
		}

		using Pg::API::Input::eKeyCode;
		if (_inputSystem->GetKeyDown(eKeyCode::MouseLeft))
		{
			PG_TRACE("마우스 왼쪽 버튼 클릭");
		}
		if (_inputSystem->GetKey(eKeyCode::MouseRight))
		{
			std::string mouseX = std::to_string(_inputSystem->GetMouseX());
			std::string mouseY = std::to_string(_inputSystem->GetMouseY());
			std::string outString = "마우스 오른쪽 버튼 클릭 중 ";
			outString.append(mouseX).append(", ").append(mouseY);
			PG_TRACE(outString);
		}
		
	}

	void EngineMain::Finalize()
	{

	}

}
