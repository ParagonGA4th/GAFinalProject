#pragma once
#include "../ParagonData/Scene.h"
#include "../ParagonData/EditorMode.h"
#include <string>

namespace Pg::Engine { class SceneSystem; }
namespace Pg::Data { class Scene; }
namespace Pg::API 
{
	class APIMain;
	namespace Graphics { class PgGraphics; }
}

namespace Pg::API
{
	class PgScene
	{
		friend class APIMain;
	public:
		void Initialize();

		void SetCurrentScene(const std::string& sceneName);

		Pg::Data::Scene* GetCurrentScene();

		std::vector<Pg::Data::Scene*> GetProjectSceneList();

		Pg::Data::Enums::eEditorMode GetEditorMode();

	private:
		//void AssignFunctionToSceneSystem();
		void UpdateLoop(); //Queue 처럼 동작해야 한다. SetCurrentScene에 한정해서.
	
		void ResetSceneBoolValues();
	
	private:
		Pg::Engine::SceneSystem* _sceneSystem = nullptr;
		Pg::API::Graphics::PgGraphics* _pgGraphics = nullptr;

		bool _shouldChangeScene{ false };

		bool _inMidstTransition{ false };
		bool _inMidstFadingOut{ false };
		bool _inMidstFadingIn{ false };
		std::string _toChangeSceneName;
	};
}

