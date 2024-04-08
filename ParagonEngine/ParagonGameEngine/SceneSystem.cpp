#include "SceneSystem.h"
#include "SoundSystem.h"
#include "PhysicSystem.h"
#include "EngineMain.h"
#include "BehaviorTreeSystem.h"
#include "EditorCameraScript.h"

//Script System 마련 전 Test 용.
#include "TestScene.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h"
#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>

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
		//_sceneList.insert({"SampleScene", new Pg::Data::Scene("SampleScene")});
		//_currentScene = _sceneList.at("SampleScene");
		///</기존SampleScene코드>
		
		///<임시Test용: TO REMOVE>
		TestScene* tTestScene = new TestScene();
		tTestScene->Initialize();

		_currentScene = tTestScene->GetCurrentScene();
		_sceneList.insert({ "TestSceneToRemove", tTestScene->GetCurrentScene() });
		///<임시Test용: TO REMOVE>
		
		//일단 별도로 Initialize할 때 기존 로직을 무너뜨리지 않기 위해서 Scene의 내용을 추가했음.
		//씬이 시작하기 전, 일괄적으로 AddObject / DeleteObject 외부에서 호출된 요소들을 반영한다.
		//모든 씬들에 대하여 설정. 일괄적으로 반영하고 시작한다.
		//이제는 런타임 추가용 함수 / 에디터타임 추가용 함수 분리되었기에 크게 문제 X.
		//없어도 문제되지 않을 것이기에, 제거.
		//std::for_each(_sceneList.begin(), _sceneList.end(), [](auto& iter)
		//	{ iter.second->HandleAddDeleteInScene(); });
	}
	
	void SceneSystem::DebounceSceneLoadStatus()
	{
		//씬 자체의 Start를 보기 위해서.
		if (!_isStarted)
		{
			///급하게 리소스를 보기 위해서 사용. 클라이언트 작업 시 무조건 삭제!!!!!
			PG_WARN("이건 여기 있어서는 안된다!!! 아쿠마다!!!!!!");
			_currentScene->GetMainCamera()->_object->AddComponent<EditorCameraScript>();
			_isStarted = true;
		}
	}

	void SceneSystem::Update()
	{
		//Object 단위로 내부적으로 실행할지 말지를 판단하기에, 상관없다.
		
		_currentScene->Awake();
		_currentScene->Start();
		_currentScene->Update();
		_currentScene->FixedUpdate();
		_currentScene->LateUpdate();

		//런타임 Add/Remove 오브젝트 관리.
		_currentScene->HandleAddDeleteInScene();
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


	void SceneSystem::SetCurrentScene(Pg::Data::Scene* scene)
	{
		//현재 씬 저장된거 바꾸기.
		_currentScene = scene;

		//다시 Start할 수 있게 Bool Toggle.
		this->_isStarted = false;

		//현재 있는 모든 Scene 내부 GameObject 다시 Awake / Start 실행 가능하게 리셋.
		std::for_each(_currentScene->GetObjectList().begin(), _currentScene->GetObjectList().end(), [](auto& iter)
			{ iter->ResetDebouncerBoolean(); });

		//씬이 바뀔 시 사운드 전부 다시 로드.
		auto& tSoundSystem = singleton<SoundSystem>();
		_soundSystem = &tSoundSystem;
		_soundSystem->SyncAudioSources();

		//충돌 객체 또한 전부 다시 로드.
		auto& tPhysicSystem = singleton<Physic::PhysicSystem>();
		_physicSystem = &tPhysicSystem;
		_physicSystem->InitMakeColliders();

		//현재 업데이트시켜야 하는 Behavior Tree List 역시 다시 로드.
		auto& tBTSystem = singleton<BTree::BehaviorTreeSystem>();
		_btSystem = &tBTSystem;
		_btSystem->SyncSceneActiveBT();
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

	bool SceneSystem::GetIsStartedScene()
	{
		return _isStarted;
	}



}