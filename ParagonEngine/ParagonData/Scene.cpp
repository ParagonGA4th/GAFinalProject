#include "Scene.h"
#include "GameObject.h"
#include <windows.h>


namespace Pg::Data
{
	Scene::Scene(std::string sceneName) :
		_sceneName(sceneName),
		_mainCamera(nullptr),
		_objectList()
	{
		//Scene이 만들어질 경우 무조건 MainCamera가 오브젝트로 생성이 되어 있어야 함!
		GameObject* cameraObject = AddObject("MainCamera");
		_mainCamera = cameraObject->AddComponent<Pg::Engine::Camera>();
		_mainCamera->_object->_transform.SetPosition({ 10.0f, 0.0f, 0.0f });
		_mainCamera->_object->_transform.SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });

		// 10.11 오수안
		// Scene이 생성될 때 반드시 추가되는 main Light
		GameObject* dirLight = AddObject("MainDirLight");
		_mainDirLight = dirLight->AddComponent<DirectionalLight>();
		_mainDirLight->_object->_transform.SetPosition(5.f, 5.f, 5.f);
		_mainDirLight->SetLightColor(1.f, 0.f, 0.f, 1.f);

		SetMainCamera(_mainCamera);
		SetMainLight(_mainDirLight);
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
		OutputDebugString(L"MainCamera Get!");
	}

	void Scene::SetMainCamera(Camera* mainCamera)
	{
		_mainCamera = mainCamera;

		// 메인 카메라가 바뀌었을 때 오브젝트를 현재
		// 메인 카메라 기준으로 바꾸어준다

		OutputDebugString(L"MainCamera Set!");
	}

	std::vector<GameObject*>& Scene::GetObjectList()
	{
		return _objectList;
	}

	DirectionalLight* Scene::GetMainLight()
	{
		return _mainDirLight;
	}

	void Scene::SetMainLight(DirectionalLight* mainLight)
	{
		_mainDirLight = mainLight;
	}

}
