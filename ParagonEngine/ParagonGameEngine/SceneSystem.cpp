#include "SceneSystem.h"
#include "SoundSystem.h"
#include "PhysicSystem.h"
#include "BehaviorTreeSystem.h"
#include "TestScene.h"
#include "EditorCameraScript.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	SceneSystem::SceneSystem() : _isStarted(false)
	{
		_testScene = new TestScene();
		_currentScene = _testScene->GetCurrentScene();
	}

	SceneSystem::~SceneSystem()
	{


	}
	void SceneSystem::Initialize()
	{
		//여기에는 (TBA) Scene 관리 로직 etc 있어야!
		_testScene->Initialize();
	}
	
	void SceneSystem::Update()
	{
		//현재 씬의 Update를 호출시켜주면 TestScene에 존재하는 Update도 호출이 된다.
		if (!_isStarted)
		{
			///급하게 리소스를 보기 위해서 사용. 클라이언트 작업 시 무조건 삭제!!!!!
			PG_WARN("이건 여기 있어서는 안된다!!! 아쿠마다!!!!!!");
			_currentScene->GetMainCamera()->_object->AddComponent<EditorCameraScript>();

			_currentScene->Start();
			_isStarted = true;
		}

		_currentScene->Update();
		_currentScene->FixedUpdate();
		_currentScene->LateUpdate();
		
	}

	void SceneSystem::LoadEmptyScene()
	{
		UnLoadSCene();

		//씬을 생성해서
		Pg::Data::Scene* scene = new Scene("Empty Scene");
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


	void SceneSystem::UnLoadSCene()
	{
		_currentScene = nullptr;
	}


	void SceneSystem::SetCurrentScene(Pg::Data::Scene* scene)
	{
		//현재 씬 저장된거 바꾸기.
		_currentScene = scene;

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

	void SceneSystem::DeleteCurrentScene()
	{
		if (_currentScene)
		{
			delete _currentScene;
			_currentScene = nullptr;
		}
	}

	void SceneSystem::SetSceneData(Pg::Data::Scene* scene)
	{
		_testScene->SetScenesData(scene);
	}
}