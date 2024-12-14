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
		//Scene�� ������� ��� ������ MainCamera�� ������Ʈ�� ������ �Ǿ� �־�� ��!
		{
			GameObject* cameraObject = AddObject("EditorCamera");
			cameraObject->SetTag("TAG_EditorCamera");
			Camera* tEditorCam = cameraObject->AddComponent<Pg::Data::Camera>();
			cameraObject->AddComponent<Pg::Data::EditorCameraScript>();

			_editorCamera = tEditorCam;
			SetMainCamera(_editorCamera);
		}
		
		//OutputDebugString(L"Scene�� �����ڿ��� �ϵ��ڵ��Ǿ� �ִ�. : ���� GameConstantData�� �޴´�. /// ���߿� �ػ� ���� �߻� �� ���ľ�.");
		
		// 10.11 ������
		// Scene�� ������ �� �ݵ�� �߰��Ǵ� main Light
		//GameObject* dirLight = AddObject("MainDirLight");
		//_mainDirLight = dirLight->AddComponent<DirectionalLight>();
		//_mainDirLight->_object->_transform.SetPosition(5.f, 5.f, 5.f);
		//_mainDirLight->SetLightColor(1.f, 0.f, 0.f, 1.f);

		//SetMainCamera(_mainCamera);
		//SetMainLight(_mainDirLight);
	}

	Scene::~Scene()
	{
		//���� �������� ������Ʈ ����Ʈ ���� ������Ʈ�� ���� �����
		for (auto& object : _objectList)
		{
			delete object;
		}

		//����Ʈ�� �����.
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
		//���߿� SceneSystem�� isAwake �������� Object�� ��Ÿ�� �߰� ����ؼ�
		//If�� �˻� �Ź� �־�� �Ѵ�. -> �ݿ���.
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
		//���߿� SceneSystem�� isStarted �������� Object�� ��Ÿ�� �߰� ����ؼ�
		//If�� �˻� �Ź� �־�� �Ѵ�. -> �ݿ���.
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
		//DontDestroyOnLoad���� �ش���� �ʴ´�.

		std::for_each(_objectList.begin(), _objectList.end(), [](auto& iter)
			{
				if (iter->GetActive())
				{
					iter->GrabManagedObjects();
				}
			});
	}

	//������ void�� ��ȯ. �ϰ������� Scene Loop ���� �����ϱ� ����.
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
		// ���� ī�޶� ����
		_mainCamera = mainCamera;

		// �׷��Ƚ��� ���޵� ī�޶� �����͸� ���� ���� ī�޶�� ����
		_mainCamera->GetCameraData();

		// ����� �α� ���
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
		//AddObject �ϰ������� ó��.
		for (auto& it : _addObjectPlanList)
		{
			GameObject* gameObj = new GameObject(it, this);

			//Unity�� GameObject.Find() ���� �����ϰ� �Ϸ��� ���ӿ�����Ʈ���ٰ� �Ҽӵ� Scene�� �޾ư� �� �ְ���.
			_objectList.push_back(gameObj);
		}

		//DeleteObject �ϰ������� ó��.
		for (auto& it : _deleteObjectPlanList)
		{
			_objectList.erase(std::remove_if(_objectList.begin(), _objectList.end(), [it](GameObject* obj)
				{
					return obj->GetName() == it;
				}));
		}

		//�� ó�� ������ Clear �ϱ�.
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

		//Unity�� GameObject.Find() ���� �����ϰ� �Ϸ��� ���ӿ�����Ʈ���ٰ� �Ҽӵ� Scene�� �޾ư� �� �ְ���.
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
