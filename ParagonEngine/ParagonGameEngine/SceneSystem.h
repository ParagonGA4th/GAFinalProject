#pragma once
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/EditorMode.h"

#include "EngineDLLExporter.h"
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
		PARAGON_ENGINE_DLL SceneSystem();
		PARAGON_ENGINE_DLL ~SceneSystem();

		void Initialize();

		//Editor 모드 도입으로 : Update가 고정적으로 호출되지 않게 되었다.
		//EditorCameraScript를 Sync에 맞춰서 넣는 로직을 별도 분리.
		void DebounceSceneLoadStatus(Pg::Data::Enums::eEditorMode editMode);
		void BeforePhysicsUpdateInGame();
		void Update(bool isActualInGame); 

		void LoadEmptyScene();
		void UnLoadSCene();
		
		PARAGON_ENGINE_DLL void SetCurrentScene(const std::string& sceneName);

		PARAGON_ENGINE_DLL Scene* GetCurrentScene();
		
		void SetSceneList(std::vector<Scene*> scenes);
		std::vector<Scene*> GetSceneList();

		//내부적으로 스크립트 형태로 존재하는 오브젝트들이 (일반 오브젝트에 한정, DontDestroyOnLoad는 해당되지 않음)
		void SetProjectSceneList_GrabManagedObjects(const std::vector<Pg::Data::Scene*>& sceneVec);
		PARAGON_ENGINE_DLL std::vector<Pg::Data::Scene*> GetProjectSceneList();
		
		//새로운 씬을 생성한다.
		Scene* CreateScene(const std::string& sceneName);

		//얘는 Scene이 멈출 때, 동작을 할 것이다. Edit Mode로 변했을 때. 
		void OnStopScene();


		//현재 씬으로 지정된 것을 삭제한다.
		void DeleteCurrentScene();

		//isStarted 반영.
		bool GetIsStartedScene();

		//Scene Change 시 중요.
		bool _isStarted;
	private:
		void SetCurrentScene_Internal(Scene* scene);
	private:
		void CheckMoveDontDestroyOnLoadObjects(Pg::Data::Scene* scene);
		void AwakeStartDontDestroyOnLoadObjects();
		void UpdateDontDestroyOnLoadObjects();
		void FixedUpdateDontDestroyOnLoadObjects();
		void LateUpdateDontDestroyOnLoadObjects();
		void UpdateActualSceneChange();

	private:
		Scene* _currentScene = nullptr;
		std::unordered_map<std::string, Scene*> _sceneList;

		//Project Scene List 따로 보관.
		std::vector<Pg::Data::Scene*> _projectSceneList;

	private:
		SoundSystem* _soundSystem = nullptr;
		Physic::PhysicSystem* _physicSystem = nullptr;
		BTree::BehaviorTreeSystem* _btSystem = nullptr;
		std::string _toChangeScene{};
		bool _isNeedToChangeScene{ false };
	};
}

