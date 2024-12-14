#include "Scene.h"
#include "GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/GameConstantData.h"
#include "../ParagonData/EditorCameraScript.h"
#include "../ParagonUtil/Log.h"
#include <algorithm>
#include <windows.h>


namespace Pg::Data
{
	std::vector<GameObject*> Scene::_dontDestroyOnList{};

	Scene::Scene(std::string sceneName) :
		_sceneName(sceneName),
		_mainCamera(nullptr),
		_objectList(),
		_graphicsDebugData(), _is3D(true)
	{
		//Scene이 만들어질 경우 무조건 MainCamera가 오브젝트로 생성이 되어 있어야 함!
		{
			GameObject* cameraObject = AddObject("EditorCamera");
			cameraObject->SetTag("TAG_EditorCamera");
			Camera* tEditorCam = cameraObject->AddComponent<Pg::Data::Camera>();
			cameraObject->AddComponent<Pg::Data::EditorCameraScript>();

			_editorCamera = tEditorCam;
			SetMainCamera(_editorCamera);
		}
		
		//OutputDebugString(L"Scene의 생성자에서 하드코딩되어 있다. : 이제 GameConstantData를 받는다. /// 나중에 해상도 변경 발생 시 고쳐야.");
		
		// 10.11 오수안
		// Scene이 생성될 때 반드시 추가되는 main Light
		//GameObject* dirLight = AddObject("MainDirLight");
		//_mainDirLight = dirLight->AddComponent<DirectionalLight>();
		//_mainDirLight->_object->_transform.SetPosition(5.f, 5.f, 5.f);
		//_mainDirLight->SetLightColor(1.f, 0.f, 0.f, 1.f);

		//SetMainCamera(_mainCamera);
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

	void Scene::Internal_EngineAwake()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->Internal_EngineAwake();
				}
			});
	}

	void Scene::BeforePhysicsAwake()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->BeforePhysicsAwake();
				}
			});
	}

	void Scene::Awake()
	{
		//나중에 SceneSystem의 isAwake 외적으로 Object의 런타임 추가 고려해서
		//If문 검사 매번 있어야 한다. -> 반영됨.
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->Awake();
				}
			});
	}

	void Scene::Start()
	{
		//나중에 SceneSystem의 isStarted 외적으로 Object의 런타임 추가 고려해서
		//If문 검사 매번 있어야 한다. -> 반영됨.
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->Start();
				}
			});
	}

	void Scene::Internal_EngineUpdate()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->Internal_EngineUpdate();
				}
			});
	}

	void Scene::BeforePhysicsUpdate()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->BeforePhysicsUpdate();
				}
			});
	}

	void Scene::Update()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->Update();
				}
			});
	}

	void Scene::OnAnimationEnd()
	{
		//const std::string justEndedAnimation
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->OnAnimationEnd();
				}
			});
	}

	void Scene::FixedUpdate()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->FixedUpdate();
				}
			});
	}

	void Scene::LateUpdate()
	{
		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{ 
				if (iter->GetActive())
				{
					iter->LateUpdate();
				}
			});
	}

	void Scene::GrabManagedObjects()
	{
		//DontDestroyOnLoad에는 해당되지 않는다.

		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->GrabManagedObjects();
				}
			});
	}

	//이제는 void를 반환. 일괄적으로 Scene Loop 기준 연산하기 때문.
	void Scene::AddObjectRuntime(const std::string& obj)
	{
		_addObjectPlanList.push_back(obj);
	}

	void Scene::DeleteObjectRuntime(const std::string& obj)
	{
		_deleteObjectPlanList.push_back(obj);
	}

	std::string Scene::GetSceneName()
	{
		return _sceneName;
	}

	std::string Scene::GetSceneNameConst() const
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

	Pg::Data::GameObject* Scene::FindObjectWithName(const std::string& name)
	{
		for (auto& it : _objectList)
		{
			if (name.compare(it->GetName()) == 0)
			{
				return it;
			}
		}

		return nullptr;
	}

	std::vector<Pg::Data::GameObject*> Scene::FindObjectsWithTag(const std::string& tag)
	{
		std::vector<Pg::Data::GameObject*> tRet;

		for (auto& it : _objectList)
		{
			if (tag.compare(it->GetTag()) == 0)
			{
				tRet.push_back(it);
			}
		}

		return tRet;
	}

	void Scene::HandleAddDeleteInScene()
	{
		//AddObject 일괄적으로 처리.
		for (auto& it : _addObjectPlanList)
		{
			GameObject* gameObj = new GameObject(it, this);

			//Unity의 GameObject.Find() 등이 가능하게 하려고 게임오브젝트에다가 소속된 Scene을 받아갈 수 있게함.
			_objectList.push_back(gameObj);
		}

		//DeleteObject 일괄적으로 처리.
		for (auto& it : _deleteObjectPlanList)
		{
			_objectList.erase(std::remove_if(_objectList.begin(), _objectList.end(), [it](GameObject* obj)
				{
					return obj->GetName() == it;
				}));
		}

		//다 처리 했으니 Clear 하기.
		if (!_addObjectPlanList.empty())
		{
			_addObjectPlanList.clear();
		}

		if (!_deleteObjectPlanList.empty())
		{
			_deleteObjectPlanList.clear();
		}
	}

	Pg::Data::GameObject* Scene::AddObject(const std::string& obj)
	{
		GameObject* gameObj = new GameObject(obj, this);

		//Unity의 GameObject.Find() 등이 가능하게 하려고 게임오브젝트에다가 소속된 Scene을 받아갈 수 있게함.
		_objectList.push_back(gameObj);

		return gameObj;
	}

	void Scene::DeleteObject(const std::string& obj)
	{
		_objectList.erase(std::remove_if(_objectList.begin(), _objectList.end(), [obj](GameObject* stored)
			{
				return stored->GetName() == obj;
			}));
	}

	Pg::Data::Camera* Scene::GetEditorCamera()
	{
		return _editorCamera;
	}

	bool Scene::GetIs3D() const
	{
		return _is3D;
	}

	void Scene::SetIs3D(bool is3d)
	{
		_is3D = is3d;
	}
}
