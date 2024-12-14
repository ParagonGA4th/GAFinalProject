#include "SceneSystem.h"
#include "SoundSystem.h"
#include "PhysicSystem.h"
#include "EngineMain.h"
#include "BehaviorTreeSystem.h"
//#include "EditorCameraScript.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h"
#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <ranges>
#include <singleton-cpp/singleton.h>
#include <cassert>
namespace Pg::Engine
{
	SceneSystem::SceneSystem() : _isStarted(false)
	{

		///이거 클라이언트로 빼면서 지우고
		///.pgproject파일을 이용해 받아와야함.
	}

	SceneSystem::~SceneSystem()
	{


	}
	void SceneSystem::Initialize()
	{
		///<기존SampleScene코드>
		////여기에는 (TBA) Scene 관리 로직 etc 있어야!
		_sceneList.insert({ "SampleScene", new Pg::Data::Scene("SampleScene") });
		_currentScene = _sceneList.at("SampleScene");
		///</기존SampleScene코드>

		//일단 별도로 Initialize할 때 기존 로직을 무너뜨리지 않기 위해서 Scene의 내용을 추가했음.
		//씬이 시작하기 전, 일괄적으로 AddObject / DeleteObject 외부에서 호출된 요소들을 반영한다.
		//모든 씬들에 대하여 설정. 일괄적으로 반영하고 시작한다.
		//이제는 런타임 추가용 함수 / 에디터타임 추가용 함수 분리되었기에 크게 문제 X.
		//없어도 문제되지 않을 것이기에, 제거.
		std::for_each(_sceneList.begin(), _sceneList.end(), [](auto& iter)
			{ iter.second->HandleAddDeleteInScene(); });
	}

	//Editor모드 받아서 검사.
	void SceneSystem::DebounceSceneLoadStatus(Pg::Data::Enums::eEditorMode editMode)
	{
		//씬 자체의 Start를 보기 위해서.
		if (!_isStarted)
		{
			/////급하게 리소스를 보기 위해서 사용. 클라이언트 작업 시 무조건 삭제!!!!!
			//PG_WARN("이건 여기 있어서는 안된다!!! 아쿠마다!!!!!!");
			//_currentScene->GetMainCamera()->_object->AddComponent<EditorCameraScript>();
			if (!(editMode == Data::Enums::eEditorMode::_NONE ||
				editMode == Data::Enums::eEditorMode::_EDIT))
			{
				//PG_ERROR("now Checking");
				CheckMoveSortDontDestroyOnLoadObjects(_currentScene);
				AwakeStartDontDestroyOnLoadObjects();

				///PLAY STOP?
				/// Play Mode일 경우 다시 호출
				auto& tPhysicSystem = singleton<Physic::PhysicSystem>();
				_physicSystem = &tPhysicSystem;
				_physicSystem->InitMakeColliders();
			}
			_isStarted = true;
		}
	}

	void SceneSystem::BeforePhysicsUpdateInGame()
	{
		_currentScene->BeforePhysicsAwake();
		_currentScene->BeforePhysicsUpdate();
	}

	bool SceneSystem::Update(bool isActualInGame)
	{
		//Object 단위로 내부적으로 실행할지 말지를 판단하기에, 상관없다.
		if (isActualInGame)
		{
			_currentScene->Internal_EngineAwake();
			_currentScene->Awake();
			_currentScene->Start();
			_currentScene->Internal_EngineUpdate();
			UpdateDontDestroyOnLoadObjects();
			_currentScene->Update();
			_currentScene->OnAnimationEnd();
			FixedUpdateDontDestroyOnLoadObjects();
			_currentScene->FixedUpdate();
			LateUpdateDontDestroyOnLoadObjects();
			_currentScene->LateUpdate();

			//런타임 Add/Remove 오브젝트 관리.
			_currentScene->HandleAddDeleteInScene();
		}
		else
		{
			_currentScene->Internal_EngineAwake();
			_currentScene->Internal_EngineUpdate();
		}

		bool tIsJustSceneChanged = UpdateActualSceneChange();
		return tIsJustSceneChanged;
	}

	void SceneSystem::LoadEmptyScene()
	{
		UnLoadSCene();

		//씬을 생성해서
		Pg::Data::Scene* scene = new Scene("Empty Scene");
	}

	void SceneSystem::UnLoadSCene()
	{
		_currentScene = nullptr;
	}


	void SceneSystem::SetCurrentScene_Internal(Pg::Data::Scene* scene)
	{
		//FadeOut Activate.
		//if (_fadeOutFunction)
		//{
		//	PG_WARN("FADING_OUT");
		//	_fadeOutFunction();
		//}

		//전에 있던 씬에서 오브젝트 클린업.
		std::for_each(_currentScene->GetObjectList().begin(), _currentScene->GetObjectList().end(), [](auto& iter)
			{iter->CleanOnSceneChange(); });

		//현재 씬 저장된거 바꾸기.
		_currentScene = scene;

		//다시 Start할 수 있게 Bool Toggle.
		this->_isStarted = false;

		//현재 있는 모든 Scene 내부 GameObject 다시 Awake / Start 실행 가능하게 리셋.
		std::for_each(_currentScene->GetObjectList().begin(), _currentScene->GetObjectList().end(), [](auto& iter)
			{ iter->ResetDebouncerBoolean(); });

		//SetCurrentScene이 호출되었을 때, Global한 애들 기준으로 (Static Vector, Don't Destroy On Load)
		//OnSceneChange_Global()을 호출한다. (일반 오브젝트들에는 해당되지 않는다)
		if (!(Pg::Data::Scene::_dontDestroyOnList.empty()))
		{
			std::for_each(Pg::Data::Scene::_dontDestroyOnList.begin(), Pg::Data::Scene::_dontDestroyOnList.end(), [&scene](auto& iter)
				{ iter->OnSceneChange_Global(scene); });
		}
		PG_TRACE("OnSceneChange Global Called");

		//씬이 바뀔 시 사운드 전부 다시 로드.
		auto& tSoundSystem = singleton<SoundSystem>();
		_soundSystem = &tSoundSystem;
		_soundSystem->SyncAudioSources();
		PG_TRACE("SyncAudioSources Called");

		//충돌 객체 또한 전부 다시 로드.
		auto& tPhysicSystem = singleton<Physic::PhysicSystem>();
		_physicSystem = &tPhysicSystem;
		_physicSystem->InitMakeColliders();
		PG_TRACE("InitMakeColliders Called");

		//현재 업데이트시켜야 하는 Behavior Tree List 역시 다시 로드.
		auto& tBTSystem = singleton<BTree::BehaviorTreeSystem>();
		_btSystem = &tBTSystem;
		_btSystem->SyncSceneActiveBT();
		PG_TRACE("SyncSceneActiveBT Called");

		//FadeIn Activate.
		//if (_fadeInFunction)
		//{
		//	PG_WARN("FADING_IN");
		//	_fadeInFunction();
		//}
	}

	void SceneSystem::SetCurrentScene(const std::string& sceneName)
	{
		_toChangeScene = sceneName;
		_isNeedToChangeScene = true;

		PG_TRACE(sceneName);

		if (sceneName == "Stage1")
		{
			assert("");
		}
	}

	Pg::Data::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

	Pg::Data::Scene* SceneSystem::CreateScene(const std::string& sceneName)
	{
		auto it = _sceneList.find(sceneName);
		if (it != _sceneList.end())
		{
			return _sceneList[sceneName];
		}

		Pg::Data::Scene* scene;
		scene = new Pg::Data::Scene(sceneName);
		return scene;
	}

	void SceneSystem::DeleteCurrentScene()
	{
		if (_currentScene)
		{
			delete _currentScene;
			_currentScene = nullptr;
		}
	}

	void SceneSystem::SetSceneList(std::vector<Scene*> scenes)
	{
		for (auto& vscene : scenes)
		{
			auto tscene = _sceneList.find(vscene->GetSceneName());
			if (tscene == _sceneList.end())
			{
				_sceneList.insert({ vscene->GetSceneName(), vscene });
			}
		}
	}

	std::vector<Scene*> SceneSystem::GetSceneList()
	{
		std::vector<Scene*> scenes;

		for (auto& vscene : _sceneList)
		{
			scenes.emplace_back(vscene.second);
		}

		return scenes;
	}

	bool SceneSystem::GetIsStartedScene()
	{
		return _isStarted;
	}

	void SceneSystem::CheckMoveSortDontDestroyOnLoadObjects(Pg::Data::Scene* scene)
	{
		std::vector<Pg::Data::GameObject*> tGlobalObjSceneList;

		for (auto& obj : scene->_objectList)
		{
			if (obj->GetDontDestroyOnLoad())
			{
				tGlobalObjSceneList.push_back(obj);
			}
		}

		//Early Return : 만약 DDOL 리스트가 비어있으면 Return하자.
		if (tGlobalObjSceneList.empty())
		{
			return;
		}

		//PG_WARN("Inside, Working");

		//Static List에서 해당 값을 찾았다는 조건 람다.
		auto tFoundFunc = [&](Pg::Data::GameObject* val)
			{
				//전체 Static DontDestroyOnLoad 리스트에서 찾았다는 얘기 -> 반대로 못 찾았으면 추가해야.
				return std::ranges::find(Pg::Data::Scene::_dontDestroyOnList, val) != Pg::Data::Scene::_dontDestroyOnList.end();
			};

		//Scene의 DontDestroyOnList에서 안 겹치면 추가.
		//Scene 사이 오갈 때 여러 객체 안 만들기 위해.
		for (auto& tGlobalObj : tGlobalObjSceneList)
		{
			//못 찾았을 때만: 
			if (!tFoundFunc(tGlobalObj))
			{
				Pg::Data::Scene::_dontDestroyOnList.push_back(tGlobalObj);
			}
		}

		//반대로 DontDestroyOnLoad 오브젝트들을 Scene의 Object 리스트에서 제거.
		for (auto& obj : scene->_objectList)
		{
			scene->_objectList.erase(std::remove_if(scene->_objectList.begin(),
				scene->_objectList.end(), tFoundFunc), scene->_objectList.end());
		}

		//이제, ISortableGlobalObject에 따른 정렬을 한다.
		if (!(Pg::Data::Scene::_dontDestroyOnList.empty()))
		{
			//PG_WARN("Starting to sort");
			//PG_WARN(std::to_string(Pg::Data::Scene::_dontDestroyOnList.size()));
			//
			//for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
			//{
			//	PG_WARN(it->GetName());
			//}

			//비어있지 않으면, sort.
			std::sort(Pg::Data::Scene::_dontDestroyOnList.begin(), Pg::Data::Scene::_dontDestroyOnList.end(),
				[](Pg::Data::GameObject*& lhs, Pg::Data::GameObject*& rhs) -> bool
				{
					//1 Global Object = 1 Manager Component 원칙에 따라서 되는 것이다.
					Pg::Data::ISortableGlobalObject* tLHS = lhs->GetComponent<Pg::Data::ISortableGlobalObject>();
					Pg::Data::ISortableGlobalObject* tRHS = rhs->GetComponent<Pg::Data::ISortableGlobalObject>();

					assert((tLHS != nullptr && tRHS != nullptr) && "Global Manager Object 내부 ISortableGlobalObject 상속 컴포넌트 못 찾음");
					return (tLHS->GetPriorityIndex() <= tRHS->GetPriorityIndex());
				});
		}
	}

	void SceneSystem::OnStopScene()
	{
		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			if (it->GetActive())
			{
				it->OnEngineStop();
			}
		}

		for (auto& obj : _currentScene->_objectList)
		{
			if (obj->GetActive())
			{
				obj->OnEngineStop();
			}
		}
	}

	void SceneSystem::AwakeStartDontDestroyOnLoadObjects()
	{
		if (Pg::Data::Scene::_dontDestroyOnList.empty())
		{
			return;
		}

		//밖에서 한번만 실행된다.
		//이는, 로직 상의 문제로,
		//Dont Destroy On Load가 바뀌었을 때, DebouncerBoolean이 바뀌도록 위치 이동.
		//for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		//{
		//	it->ResetDebouncerBoolean();
		//}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->Awake();
		}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->Start();
		}
	}

	void SceneSystem::UpdateDontDestroyOnLoadObjects()
	{
		if (Pg::Data::Scene::_dontDestroyOnList.empty())
		{
			return;
		}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->Update();
		}
	}

	void SceneSystem::FixedUpdateDontDestroyOnLoadObjects()
	{
		if (Pg::Data::Scene::_dontDestroyOnList.empty())
		{
			return;
		}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->FixedUpdate();
		}
	}

	void SceneSystem::LateUpdateDontDestroyOnLoadObjects()
	{
		if (Pg::Data::Scene::_dontDestroyOnList.empty())
		{
			return;
		}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->LateUpdate();
		}
	}

	bool SceneSystem::UpdateActualSceneChange()
	{
		if (_isNeedToChangeScene)
		{
			for (auto& iter : _sceneList)
			{
				if (iter.first == _toChangeScene)
				{
					SetCurrentScene_Internal(iter.second);

					if (!_toChangeScene.empty())
					{
						_toChangeScene.clear();
					}
					_isNeedToChangeScene = false;

					//지금 당장 Scene이 변했다는 말.
					return true;
				}
			}
			assert(false && "SceneName과 동일한 Scene이 존재하지 않음.");
		}

		//지금 당장 Scene이 바뀌지 않았다는 말.
		return false;
	}

	void SceneSystem::SetProjectSceneList_GrabManagedObjects(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		if (!_projectSceneList.empty())
		{
			//만약 비어있지 않았다면 Clear.
			_projectSceneList.clear();
		}

		//옮기기.
		std::copy(sceneVec.begin(), sceneVec.end(), std::back_inserter(_projectSceneList));

		//동시에, 새로운 Manager들에 한정하여, 다른 동작을 하지 말고,
		//단순히 그들을 모아둘 수단이 필요하다. (플로우 관리를 위해)
		for (auto& it : _projectSceneList)
		{
			//내부적으로 GrabManagedObjects (매니저들이 관리하는 대상 등등, 모아두기 위해)
			//카메라 같은 애들은 내부적으로 불가.
			it->GrabManagedObjects();
		}

		//Don't Destroy On Load를 실제로 옮겨야 할 것이다..
		//OnSceneChange_Global을 실행해야 하기 때문.
		//Project 기준 0번째 인덱스의 씬에서 Don't Destroy On Load 오브젝트들 옮김.
		//Ex. TotalGameManager등을 연동하기 위해서.
		CheckMoveSortDontDestroyOnLoadObjects(sceneVec.at(0));

		//이로서 Don't Destroy On Load는 0번째 씬에서밖에 있을 수 없다.
		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->OnSceneChange_Global(sceneVec.at(0));
		}

		for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
		{
			it->ResetDebouncerBoolean();
		}
	}

	std::vector<Pg::Data::Scene*> SceneSystem::GetProjectSceneList()
	{
		//복사된 버전을 반환.
		return _projectSceneList;
	}

	Pg::Data::Enums::eEditorMode SceneSystem::GetEditorMode()
	{
		return _justStoreMode;
	}

	void SceneSystem::PassEditorModeForStoring(Pg::Data::Enums::eEditorMode mode)
	{
		_justStoreMode = mode;
	}

	//void SceneSystem::AssignFunctionToSceneSystem(std::function<void()> fadeInFunction, std::function<void()> fadeOutFunction)
	//{
	//	_fadeInFunction = fadeInFunction;
	//	_fadeOutFunction = fadeOutFunction;
	//	assert(_fadeInFunction);
	//	assert(_fadeOutFunction);
	//}

}