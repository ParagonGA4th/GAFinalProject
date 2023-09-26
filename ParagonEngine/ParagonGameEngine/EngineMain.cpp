#include "EngineMain.h"
#include "InputSystem.h"
#include "EngineResourceManager.h"

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

namespace Pg::Engine
{
	EngineMain::EngineMain(Pg::Core::CoreMain* core) : _coreMain(core), _engineResourceManager(nullptr)
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

	Pg::Engine::Manager::EngineResourceManager* EngineMain::GetEngineResourceManager()
	{
		if (this->_engineResourceManager == nullptr)
		{
			this->_engineResourceManager = Pg::Engine::Manager::EngineResourceManager::Instance();
		}
		return _engineResourceManager;
	}
	
	

}
