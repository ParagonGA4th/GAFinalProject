#include "PgScene.h"
#include "../ParagonGameEngine/SceneSystem.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API
{
	void PgScene::Initialize()
	{
		auto& tSceneSystem = singleton<Pg::Engine::SceneSystem>();
		_sceneSystem = &tSceneSystem;
	}

	void PgScene::SetCurrentScene(const std::string& sceneName)
	{
		_sceneSystem->SetCurrentScene(sceneName);
	}
}



