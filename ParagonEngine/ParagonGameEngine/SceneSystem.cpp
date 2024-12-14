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

		///�̰� Ŭ���̾�Ʈ�� ���鼭 �����
		///.pgproject������ �̿��� �޾ƿ;���.
	}

	SceneSystem::~SceneSystem()
	{


	}
	void SceneSystem::Initialize()
	{
		///<����SampleScene�ڵ�>
		////���⿡�� (TBA) Scene ���� ���� etc �־��!
		_sceneList.insert({ "SampleScene", new Pg::Data::Scene("SampleScene") });
		_currentScene = _sceneList.at("SampleScene");
		///</����SampleScene�ڵ�>

		//�ϴ� ������ Initialize�� �� ���� ������ ���ʶ߸��� �ʱ� ���ؼ� Scene�� ������ �߰�����.
		//���� �����ϱ� ��, �ϰ������� AddObject / DeleteObject �ܺο��� ȣ��� ��ҵ��� �ݿ��Ѵ�.
		//��� ���鿡 ���Ͽ� ����. �ϰ������� �ݿ��ϰ� �����Ѵ�.
		//������ ��Ÿ�� �߰��� �Լ� / ������Ÿ�� �߰��� �Լ� �и��Ǿ��⿡ ũ�� ���� X.
		//��� �������� ���� ���̱⿡, ����.
		std::for_each(_sceneList.begin(), _sceneList.end(), [](auto& iter)
			{ iter.second->HandleAddDeleteInScene(); });
	}

	//Editor��� �޾Ƽ� �˻�.
	void SceneSystem::DebounceSceneLoadStatus(Pg::Data::Enums::eEditorMode editMode)
	{
		//�� ��ü�� Start�� ���� ���ؼ�.
		if (!_isStarted)
		{
			/////���ϰ� ���ҽ��� ���� ���ؼ� ���. Ŭ���̾�Ʈ �۾� �� ������ ����!!!!!
			//PG_WARN("�̰� ���� �־�� �ȵȴ�!!! ������!!!!!!");
			//_currentScene->GetMainCamera()->_object->AddComponent<EditorCameraScript>();
			if (!(editMode == Data::Enums::eEditorMode::_NONE ||
				editMode == Data::Enums::eEditorMode::_EDIT))
			{
				//PG_ERROR("now Checking");
				CheckMoveSortDontDestroyOnLoadObjects(_currentScene);
				AwakeStartDontDestroyOnLoadObjects();

				///PLAY STOP?
				/// Play Mode�� ��� �ٽ� ȣ��
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
		//Object ������ ���������� �������� ������ �Ǵ��ϱ⿡, �������.
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

			//��Ÿ�� Add/Remove ������Ʈ ����.
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

		//���� �����ؼ�
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

		//���� �ִ� ������ ������Ʈ Ŭ����.
		std::for_each(_currentScene->GetObjectList().begin(), _currentScene->GetObjectList().end(), [](auto& iter)
			{iter->CleanOnSceneChange(); });

		//���� �� ����Ȱ� �ٲٱ�.
		_currentScene = scene;

		//�ٽ� Start�� �� �ְ� Bool Toggle.
		this->_isStarted = false;

		//���� �ִ� ��� Scene ���� GameObject �ٽ� Awake / Start ���� �����ϰ� ����.
		std::for_each(_currentScene->GetObjectList().begin(), _currentScene->GetObjectList().end(), [](auto& iter)
			{ iter->ResetDebouncerBoolean(); });

		//SetCurrentScene�� ȣ��Ǿ��� ��, Global�� �ֵ� �������� (Static Vector, Don't Destroy On Load)
		//OnSceneChange_Global()�� ȣ���Ѵ�. (�Ϲ� ������Ʈ�鿡�� �ش���� �ʴ´�)
		if (!(Pg::Data::Scene::_dontDestroyOnList.empty()))
		{
			std::for_each(Pg::Data::Scene::_dontDestroyOnList.begin(), Pg::Data::Scene::_dontDestroyOnList.end(), [&scene](auto& iter)
				{ iter->OnSceneChange_Global(scene); });
		}
		PG_TRACE("OnSceneChange Global Called");

		//���� �ٲ� �� ���� ���� �ٽ� �ε�.
		auto& tSoundSystem = singleton<SoundSystem>();
		_soundSystem = &tSoundSystem;
		_soundSystem->SyncAudioSources();
		PG_TRACE("SyncAudioSources Called");

		//�浹 ��ü ���� ���� �ٽ� �ε�.
		auto& tPhysicSystem = singleton<Physic::PhysicSystem>();
		_physicSystem = &tPhysicSystem;
		_physicSystem->InitMakeColliders();
		PG_TRACE("InitMakeColliders Called");

		//���� ������Ʈ���Ѿ� �ϴ� Behavior Tree List ���� �ٽ� �ε�.
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

		//Early Return : ���� DDOL ����Ʈ�� ��������� Return����.
		if (tGlobalObjSceneList.empty())
		{
			return;
		}

		//PG_WARN("Inside, Working");

		//Static List���� �ش� ���� ã�Ҵٴ� ���� ����.
		auto tFoundFunc = [&](Pg::Data::GameObject* val)
			{
				//��ü Static DontDestroyOnLoad ����Ʈ���� ã�Ҵٴ� ��� -> �ݴ�� �� ã������ �߰��ؾ�.
				return std::ranges::find(Pg::Data::Scene::_dontDestroyOnList, val) != Pg::Data::Scene::_dontDestroyOnList.end();
			};

		//Scene�� DontDestroyOnList���� �� ��ġ�� �߰�.
		//Scene ���� ���� �� ���� ��ü �� ����� ����.
		for (auto& tGlobalObj : tGlobalObjSceneList)
		{
			//�� ã���� ����: 
			if (!tFoundFunc(tGlobalObj))
			{
				Pg::Data::Scene::_dontDestroyOnList.push_back(tGlobalObj);
			}
		}

		//�ݴ�� DontDestroyOnLoad ������Ʈ���� Scene�� Object ����Ʈ���� ����.
		for (auto& obj : scene->_objectList)
		{
			scene->_objectList.erase(std::remove_if(scene->_objectList.begin(),
				scene->_objectList.end(), tFoundFunc), scene->_objectList.end());
		}

		//����, ISortableGlobalObject�� ���� ������ �Ѵ�.
		if (!(Pg::Data::Scene::_dontDestroyOnList.empty()))
		{
			//PG_WARN("Starting to sort");
			//PG_WARN(std::to_string(Pg::Data::Scene::_dontDestroyOnList.size()));
			//
			//for (auto& it : Pg::Data::Scene::_dontDestroyOnList)
			//{
			//	PG_WARN(it->GetName());
			//}

			//������� ������, sort.
			std::sort(Pg::Data::Scene::_dontDestroyOnList.begin(), Pg::Data::Scene::_dontDestroyOnList.end(),
				[](Pg::Data::GameObject*& lhs, Pg::Data::GameObject*& rhs) -> bool
				{
					//1 Global Object = 1 Manager Component ��Ģ�� ���� �Ǵ� ���̴�.
					Pg::Data::ISortableGlobalObject* tLHS = lhs->GetComponent<Pg::Data::ISortableGlobalObject>();
					Pg::Data::ISortableGlobalObject* tRHS = rhs->GetComponent<Pg::Data::ISortableGlobalObject>();

					assert((tLHS != nullptr && tRHS != nullptr) && "Global Manager Object ���� ISortableGlobalObject ��� ������Ʈ �� ã��");
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

		//�ۿ��� �ѹ��� ����ȴ�.
		//�̴�, ���� ���� ������,
		//Dont Destroy On Load�� �ٲ���� ��, DebouncerBoolean�� �ٲ�� ��ġ �̵�.
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

					//���� ���� Scene�� ���ߴٴ� ��.
					return true;
				}
			}
			assert(false && "SceneName�� ������ Scene�� �������� ����.");
		}

		//���� ���� Scene�� �ٲ��� �ʾҴٴ� ��.
		return false;
	}

	void SceneSystem::SetProjectSceneList_GrabManagedObjects(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		if (!_projectSceneList.empty())
		{
			//���� ������� �ʾҴٸ� Clear.
			_projectSceneList.clear();
		}

		//�ű��.
		std::copy(sceneVec.begin(), sceneVec.end(), std::back_inserter(_projectSceneList));

		//���ÿ�, ���ο� Manager�鿡 �����Ͽ�, �ٸ� ������ ���� ����,
		//�ܼ��� �׵��� ��Ƶ� ������ �ʿ��ϴ�. (�÷ο� ������ ����)
		for (auto& it : _projectSceneList)
		{
			//���������� GrabManagedObjects (�Ŵ������� �����ϴ� ��� ���, ��Ƶα� ����)
			//ī�޶� ���� �ֵ��� ���������� �Ұ�.
			it->GrabManagedObjects();
		}

		//Don't Destroy On Load�� ������ �Űܾ� �� ���̴�..
		//OnSceneChange_Global�� �����ؾ� �ϱ� ����.
		//Project ���� 0��° �ε����� ������ Don't Destroy On Load ������Ʈ�� �ű�.
		//Ex. TotalGameManager���� �����ϱ� ���ؼ�.
		CheckMoveSortDontDestroyOnLoadObjects(sceneVec.at(0));

		//�̷μ� Don't Destroy On Load�� 0��° �������ۿ� ���� �� ����.
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
		//����� ������ ��ȯ.
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