#include "Scene.h"
#include "GameObject.h"
#include "../ParagonData/Camera.h"

#include <algorithm>
#include <windows.h>


namespace Pg::Data
{
	Scene::Scene(std::string sceneName) :
		_sceneName(sceneName),
		_mainCamera(nullptr),
		_objectList(),
		_graphicsDebugData()
	{
		//Scene이 만들어질 경우 무조건 MainCamera가 오브젝트로 생성이 되어 있어야 함!
		GameObject* cameraObject = AddObject("MainCamera");
		_mainCamera = cameraObject->AddComponent<Pg::Data::Camera>();

		_mainCamera->_object->_transform._position = { 0.f, 3.0f, -10.f };
		_mainCamera->_object->_transform._rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
		_mainCamera->SetScreenSize(1920.0f, 1080.0f);

		OutputDebugString(L"1920x1080으로 사이즈 전달을 위해 Scene의 생성자에서 하드코딩되어 있다. 나중에 해상도 변경 발생 시 고쳐야.");
		
		// 10.11 오수안
		// Scene이 생성될 때 반드시 추가되는 main Light
		//GameObject* dirLight = AddObject("MainDirLight");
		//_mainDirLight = dirLight->AddComponent<DirectionalLight>();
		//_mainDirLight->_object->_transform.SetPosition(5.f, 5.f, 5.f);
		//_mainDirLight->SetLightColor(1.f, 0.f, 0.f, 1.f);

		SetMainCamera(_mainCamera);
		//SetMainLight(_mainDirLight);
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

	void Scene::FixedUpdate()
	{
		for (auto& object : _objectList)
		{
			object->FixedUpdate();
		}
	}

	void Scene::LateUpdate()
	{
		for (auto& object : _objectList)
		{
			object->LateUpdate();
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
		auto object = std::find_if(_objectList.begin(), _objectList.end(),
			[&](GameObject* tobj)
			{
				if (tobj->GetName() == obj) return true;
			});

		if (object != _objectList.end())
		{
			_objectList.erase(object);
		}
	}

	std::string Scene::GetSceneName()
	{
		return _sceneName;
	}

	void Scene::SetSceneName(const std::string& sceneName)
	{
		_sceneName = sceneName;
	}

	Pg::Data::Camera* Scene::GetMainCamera()
	{
		return _mainCamera;
		OutputDebugString(L"MainCamera Get!");
	}

	void Scene::SetMainCamera(Camera* mainCamera)
	{
		// 메인 카메라 변경
		_mainCamera = mainCamera;

		// 그래픽스에 전달될 카메라 데이터를 현재 메인 카메라로 변경
		_mainCamera->GetCameraData();

		// 디버그 로그 출력
		OutputDebugString(L"MainCamera Set!");
	}

	const std::vector<GameObject*>& Scene::GetObjectList() const
	{
		return _objectList;
	}

	Pg::Data::DirectionalLight* Scene::GetMainLight()
	{
		return _mainDirLight;
	}

	void Scene::SetMainLight(DirectionalLight* mainLight)
	{
		_mainDirLight = mainLight;
	}
}
