#include "WorkSpace.h"
#include "TestScene.h"

namespace Pg::Engine
{
	void WorkSpace::Initialize()
	{
		TestScene _testScene;
		_testScene.Initialize();
	}
}
