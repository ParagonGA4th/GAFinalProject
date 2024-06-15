#pragma once
#include "../ParagonData/Scene.h"
#include <string>

namespace Pg::Engine
{
	class SceneSystem;
}

namespace Pg::Data
{
	class Scene;
}

namespace Pg::API
{
	class PgScene
	{
	public:
		void Initialize();

		void SetCurrentScene(const std::string& sceneName);

		Pg::Data::Scene* GetCurrentScene();

		std::vector<Pg::Data::Scene*> GetProjectSceneList();

	private:
		Pg::Engine::SceneSystem* _sceneSystem = nullptr;
	};
}

