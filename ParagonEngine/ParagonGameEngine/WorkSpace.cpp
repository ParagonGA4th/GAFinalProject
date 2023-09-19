#include "WorkSpace.h"
#include "TestScene.h"

namespace Pg::Engine
{
	void WorkSpace::Initialize()
	{
		_testScene = new TestScene();
		_testScene->Initialize();
	}

	Pg::Core::Scene* WorkSpace::GetCurrentScene()
	{
		return _testScene->GetCurrentScene();
	}

}
