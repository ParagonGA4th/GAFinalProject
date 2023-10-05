#include "EngineMain.h"
#include "InputSystem.h"
#include "EngineResourceManager.h"

//<실제 GameEngine Resource의 목록>

//</>

#include "../ParagonCore/CoreMain.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonAPI/KeyCodeType.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

namespace Pg::Engine
{
	EngineMain::EngineMain(Pg::Core::CoreMain* core) : _coreMain(core), _engineResourceManager(Manager::EngineResourceManager::Instance())
	{
		auto& tInputSystem = singleton<Input::InputSystem>();
		_inputSystem = &tInputSystem;
	}

	EngineMain::~EngineMain()
	{

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
			PG_TRACE("Debugger Used In ParagonGameEngine!");
			tTest = true;
		}
		
	}

	void EngineMain::Finalize()
	{

	}

	void EngineMain::LoadResource(const std::string& filePath, Pg::Core::Enums::eAssetDefine define)
	{

	}

	void EngineMain::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자!
	}
	

}
