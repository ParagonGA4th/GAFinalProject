#include "WorkSpace.h"
#include "TestScene.h"
#include <windows.h>

namespace Pg::Engine
{
	void WorkSpace::Initialize()
	{
		_testScene = new TestScene();
		_testScene->Initialize();
		OutputDebugString(L"Enter WorkSpace!");
	}

	Pg::Core::Scene* WorkSpace::GetCurrentScene()
	{
		return _testScene->GetCurrentScene();
	}

}
