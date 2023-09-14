#include "Scene.h"
#include "GameObject.h"

namespace Pg::Core
{
	Scene::Scene(std::string sceneName) :
		_sceneName(sceneName),
		_objectList()
	{

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

	void Scene::AddScene(GameObject* obj)
	{

	}

	void Scene::DeleteScene(GameObject* obj)
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

	}

}
