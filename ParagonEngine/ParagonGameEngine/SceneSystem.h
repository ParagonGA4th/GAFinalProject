#pragma once
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/EditorMode.h"
#include <unordered_map>
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

	class SoundSystem;

	namespace Physic
	{
		class PhysicSystem;
	}

	namespace BTree
	{
		class BehaviorTreeSystem;
	}

	class SceneSystem
	{
	public:
		SceneSystem();
		~SceneSystem();

		void Initialize();

		//Editor 모드 도입으로 : Update가 고정적으로 호출되지 않게 되었다.
		//EditorCameraScript를 Sync에 맞춰서 넣는 로직을 별도 분리.
		void DebounceSceneLoadStatus(Pg::Data::Enums::eEditorMode editMode);
		void BeforePhysicsUpdateInGame();
		void Update(bool isActualInGame); 

		void LoadEmptyScene();
		void UnLoadSCene();
		void SetCurrentScene(Scene* scene);

		Scene* GetCurrentScene();		
		
		void SetSceneList(std::vector<Scene*> scenes);
		//Scene* GetSceneList();

		//새로운 씬을 생성한다.
		Scene* CreateScene(const std::string& sceneName);

		//현재 씬으로 지정된 것을 삭제한다.
		void DeleteCurrentScene();

		//isStarted 반영.
		bool GetIsStartedScene();

		//Scene Change 시 중요.
		bool _isStarted;

	private:
		void CheckMoveDontDestroyOnLoadObjects(Pg::Data::Scene* scene);


	private:
		Scene* _currentScene = nullptr;
		std::unordered_map<std::string, Scene*> _sceneList;

	private:
		SoundSystem* _soundSystem = nullptr;
		Physic::PhysicSystem* _physicSystem = nullptr;
		BTree::BehaviorTreeSystem* _btSystem = nullptr;
	};
}

