#pragma once
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/Scene.h"
#include <vector>

/// <summary>
/// 변지상의 SceneSystem.
/// Scene의 흐름에 관련된 로직이 모두 들어간다.
/// 2023.10.09
/// </summary>

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Engine
{
	class TestScene;
}

namespace Pg::Engine
{
	using namespace Pg::Data;

	class SceneSystem
	{
	public:
		void Initialize();

		void Update(); 

		void LoadEmptyScene();
		void UnLoadSCene();
		void SetCurrentScene(Scene* scene);
		Pg::Data::Scene* GetCurrentScene();

		//현재 씬으로 지정된 것을 삭제한다.
		void DeleteCurrentScene();


	private:
		Scene* _currentScene = nullptr;
		std::vector<Scene*> _sceneList;

		TestScene* _testScene = nullptr;
	};
}

