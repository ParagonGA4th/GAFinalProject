#include "WorkSpace.h"
#include "TestScene.h"
#include "../ParagonUtil/Log.h"

#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG


namespace Pg::Engine
{
	void WorkSpace::Initialize()
	{
		_testScene = new TestScene();
		_testScene->Initialize();
		PG_TRACE("Enter WorkSpace!");
	}

	Pg::Core::Scene* WorkSpace::GetCurrentScene()
	{
		return _testScene->GetCurrentScene();
	}

}
