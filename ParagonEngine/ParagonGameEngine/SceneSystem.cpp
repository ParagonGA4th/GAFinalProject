#include "SceneSystem.h"
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
		using namespace Pg::Data;

		Scene* tCurrentScene0 = new Scene("TestCurrentScene0");
		_currentScene = tCurrentScene0;

		PG_TRACE(_currentScene->GetSceneName());

		Scene* tCurrentScene = new Scene("TestCurrentScene");
		_currentScene = tCurrentScene;

		//이거 호출하면 MainCamera 반환함!!
		tCurrentScene->GetMainCamera();

		//카메라 하나 더 생성
		GameObject* tObj1 = tCurrentScene->AddObject("Camera1");
		tObj1->AddComponent<Camera>();
		tObj1->GetComponent<Transform>()->SetPosition({ 10.0f, 0.0f, 5.0f });

		GameObject* tObj2 = tCurrentScene->AddObject("Cube2");
		tObj2->GetComponent<Transform>()->SetPosition({ -10.0f, 0.0f, 5.0f });
		tObj2->AddComponent<RendererBase3D>();
		////tObj2->AddComponent<BaseRenderer>();
		//tObj2->AddComponent<RendererBase2D>();

		//이렇게 하면 메인 카메라 바뀜!!
		tCurrentScene->SetMainCamera(tObj1->GetComponent<Camera>());

		PG_TRACE(_currentScene->GetSceneName());


	}

	void SceneSystem::Update()
	{

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






