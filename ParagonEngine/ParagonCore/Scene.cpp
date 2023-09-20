#include "Scene.h"
#include "GameObject.h"


namespace Pg::Core
{
	Scene::Scene(std::string sceneName) :
		_sceneName(sceneName),
		_mainCamera(nullptr),
		_objectList()
	{
		//Scene이 만들어질 경우 무조건 MainCamera가 오브젝트로 생성이 되어 있어야 함!
		GameObject* cameraObject = CreateGameObject("MainCamera");
		_mainCamera = cameraObject->AddComponent<Pg::Engine::Camera>();
		_mainCamera->_object->_transform.SetPosition({ 10.0f, 0.0f, 0.0f });
		_mainCamera->_object->_transform.SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });

		SetMainCamera(_mainCamera);
	}

	Scene::~Scene()
	{
		//씬이 없어지면 오브젝트 리스트 내의 오브젝트를 전부 지우고
		for (auto& object : _objectList)
		{
			delete object;
		}

		//리스트도 지운다.
		_objectList.clear();
	}

	void Scene::Start()
	{
		for (auto& object : _objectList)
		{
			object->Start();
		}
	}

	void Scene::Update()
	{
		for (auto& object : _objectList)
		{
			object->Update();
		}
	}

	GameObject* Scene::AddObject(std::string obj)
	{
		GameObject* gameObj = new GameObject(obj);
		_objectList.push_back(gameObj);
		return gameObj;
	}

	void Scene::DeleteObject(std::string obj)
	{
		
	}

	GameObject* Scene::CreateGameObject(std::string name)
	{
		GameObject* object = new GameObject(name);
		_objectList.push_back(object);

		return object;
	}

	std::string Scene::GetSceneName()
	{
		return _sceneName;
	}

	void Scene::SetSceneName(const std::string& sceneName)
	{
		_sceneName = sceneName;
	}

	Pg::Engine::Camera* Scene::GetMainCamera()
	{
		return _mainCamera;
	}

	void Scene::SetMainCamera(Camera* mainCamera)
	{
		_mainCamera = mainCamera;
	}
}
