#include "SceneSystem.h"
#include "TestScene.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Engine
{
	void SceneSystem::Initialize()
	{
		// #Temporary: 임시로 원래 여기 이거 있으면 안되는데, 작동한다는 것 알기 위해!
		// 제거 되어야 함. 여기에는 Scene 관리 로직 etc 있어야!
		_testScene = new TestScene();
		_currentScene = _testScene->GetCurrentScene();
		_testScene->Initialize();
		_currentScene->Start();
	}
	
	void SceneSystem::Update()
	{
		//현재 씬의 Update를 호출시켜주면 TestScene에 존재하는 Update도 호출이 된다.
		_currentScene->Update();
		_currentScene->FixedUpdate();
		_currentScene->LateUpdate();
		
	}

	void SceneSystem::LoadEmptyScene()
	{
		UnLoadSCene();

		//씬을 생성해서
		Pg::Data::Scene* scene = new Scene("Empty Scene");

		//리스트에 집어 넣는다.
		_sceneList.push_back(scene);
	}


	void SceneSystem::UnLoadSCene()
	{
		_currentScene = nullptr;
	}


	void SceneSystem::SetCurrentScene(Pg::Data::Scene* scene)
	{
		_currentScene = scene;
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

}






