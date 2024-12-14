#pragma once
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/ISortableGlobalObject.h"
#include "../ParagonData/EditorMode.h"

#include "EngineDLLExporter.h"
#include <unordered_map>
#include <vector>
#include <functional>

/// <summary>
/// �������� SceneSystem.
/// Scene�� �帧�� ���õ� ������ ��� ����.
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

		//Editor ��� �������� : Update�� ���������� ȣ����� �ʰ� �Ǿ���.
		//EditorCameraScript�� Sync�� ���缭 �ִ� ������ ���� �и�.
		void DebounceSceneLoadStatus(Pg::Data::Enums::eEditorMode editMode);
		void BeforePhysicsUpdateInGame();
		bool Update(bool isActualInGame); 

		void LoadEmptyScene();
		void UnLoadSCene();
		
		PARAGON_ENGINE_DLL void SetCurrentScene(const std::string& sceneName);

		PARAGON_ENGINE_DLL Scene* GetCurrentScene();
		
		void SetSceneList(std::vector<Scene*> scenes);
		std::vector<Scene*> GetSceneList();

		//���������� ��ũ��Ʈ ���·� �����ϴ� ������Ʈ���� (�Ϲ� ������Ʈ�� ����, DontDestroyOnLoad�� �ش���� ����)
		void SetProjectSceneList_GrabManagedObjects(const std::vector<Pg::Data::Scene*>& sceneVec);
		PARAGON_ENGINE_DLL std::vector<Pg::Data::Scene*> GetProjectSceneList();

		void PassEditorModeForStoring(Pg::Data::Enums::eEditorMode mode);

		PARAGON_ENGINE_DLL Pg::Data::Enums::eEditorMode GetEditorMode();
		//PARAGON_ENGINE_DLL void AssignFunctionToSceneSystem(std::function<void()> fadeInFunction, std::function<void()> fadeOutFunction);
		
		//���ο� ���� �����Ѵ�.
		Scene* CreateScene(const std::string& sceneName);

		//��� Scene�� ���� ��, ������ �� ���̴�. Edit Mode�� ������ ��. 
		void OnStopScene();

		//���� ������ ������ ���� �����Ѵ�.
		void DeleteCurrentScene();

		//isStarted �ݿ�.
		bool GetIsStartedScene();

		//Scene Change �� �߿�.
		bool _isStarted;
	private:
		void SetCurrentScene_Internal(Scene* scene);
	private:
		void CheckMoveSortDontDestroyOnLoadObjects(Pg::Data::Scene* scene);
		void AwakeStartDontDestroyOnLoadObjects();
		void UpdateDontDestroyOnLoadObjects();
		void FixedUpdateDontDestroyOnLoadObjects();
		void LateUpdateDontDestroyOnLoadObjects();
		bool UpdateActualSceneChange();

	private:
		Scene* _currentScene = nullptr;
		std::unordered_map<std::string, Scene*> _sceneList;

		//Project Scene List ���� ����.
		std::vector<Pg::Data::Scene*> _projectSceneList;

	private:
		SoundSystem* _soundSystem = nullptr;
		Physic::PhysicSystem* _physicSystem = nullptr;
		BTree::BehaviorTreeSystem* _btSystem = nullptr;
		std::string _toChangeScene{};
		bool _isNeedToChangeScene{ false };
		Pg::Data::Enums::eEditorMode _justStoreMode{ Pg::Data::Enums::eEditorMode::_NONE };

		//std::function<void()> _fadeInFunction;
		//std::function<void()> _fadeOutFunction;
	};
}

