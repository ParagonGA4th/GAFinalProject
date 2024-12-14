#pragma once
#include <string>
#include <vector>
#include "DirectionalLight.h"
#include "../ParagonData/GraphicsDebugData.h"

/// <summary>
/// �������� Scene Ŭ����.
/// ���⼭�� �⺻���� �� ���� �� ����. �׸��� ���� ������ �ִ� ����Ʈ�� ����.
/// 2023.09.14
/// 
/// 
/// 23.10.11 ������
/// ���� �⺻������ ������ �ִ� main Directional Light �޼��带 �߰�
/// </summary>

namespace Pg::Engine
{
	class SceneSystem;
}
namespace Pg::Data
{
	class GameObject;
	class Camera;

	using Pg::Data::GameObject;
	using Pg::Data::Camera;
	using Pg::Data::DirectionalLight;

	class Scene
	{
		friend class Pg::Engine::SceneSystem;

	public:
		Scene(std::string sceneName);
		virtual ~Scene();

		//Internal_NNN : ���� �����̶� �и��� ���� ���� ���� �����ϱ� ����. (GameLoop w/ Editor Only)
		//�����͸� ������ ������ �������� PlayMode�� �ƴ� ���,
		//�䱸�Ǵ� ������ �� �� ���� �ִ�. �̶� Ȱ���ϱ� ����. ���� ���� �����̶� �и��Ǿ�� �Ѵ�.
		void Internal_EngineAwake();
		void BeforePhysicsAwake();
		void Awake();
		void Start();

		void Internal_EngineUpdate(); //Internal�� Update ����. Ex. Camera � ���� Ȱ��.
		void BeforePhysicsUpdate();	  //���� ����������, PhysicsSystem�� ���ư��� ���� Update�� ������ ���� ��. Awake���ٵ� ������ ȣ��!
		void Update();
		void OnAnimationEnd();
		void FixedUpdate();
		void LateUpdate();

		//���� Update Loop�̶� ������ �۵�, ProjectLoad�� �� ���� �۵�.
		void GrabManagedObjects();

		void SetIs3D(bool is3d);
		bool GetIs3D() const;
	
		//Editor / TestScene�� �ߵ��Ǳ� ���� �ʿ��� (������Ʈ "������ �ð�" ����) �Լ���.
		GameObject* AddObject(const std::string& obj);
		void DeleteObject(const std::string& obj);

		//��Ÿ�ӿ� ������Ʈ�� �߰� / �����ϱ� ���ؼ� ���̴� �Լ���. (������Ʈ "�� Ÿ��" ����) �Լ���.
		void AddObjectRuntime(const std::string& obj);
		void DeleteObjectRuntime(const std::string& obj);

		std::string GetSceneName();
		std::string GetSceneNameConst() const;
		void SetSceneName(const std::string& sceneName);

		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

		Camera* GetEditorCamera();

		DirectionalLight* GetMainLight();
		void SetMainLight(DirectionalLight* mainLight);

		//������Ʈ ����Ʈ ��ü�� ��ȯ.
		const std::vector<GameObject*>& GetObjectList() const;

		//������Ʈ�� �̸��� ������ ���� ������Ʈ�� ã�´�.
		Pg::Data::GameObject* FindObjectWithName(const std::string& name);

		//�±׸� ���� ������Ʈ���� ã�Ƽ� ��ȯ�Ѵ�.
		std::vector<Pg::Data::GameObject*> FindObjectsWithTag(const std::string& tag);
		
		template <typename T>
		T* FindSingleComponentInScene();

		//�ı����� �ʴ� ������Ʈ���� SceneSystem���� ���� �������� ������Ʈ�� "�Ҽ�"�� SceneSystem���� ����.
		//���� ���� �Ҽ� ������Ʈ���� ������ ���������� Pg::Data::ISortableGlobalObject�� ��ӹ޾ƾ� �Ѵ�.
		static std::vector<GameObject*> _dontDestroyOnList; 

		//�̰� Lightmaps Vector. Scene���� �̷� ������ ���;� �Ѵ�.
		std::vector<std::string> _lightmapNamesVector;

	private:
		//�� �̸�
		std::string _sceneName;

		//���� 2D ��� ������, 3D ��� �������� ����. 
		bool _is3D{ true };

		//�� �ȿ� ������Ʈ�� ������ �����Ѵ�.
		std::vector<GameObject*> _objectList;

		//ī�޶�
		Camera* _mainCamera;
		Camera* _editorCamera;
		DirectionalLight* _mainDirLight;

	public:
		GraphicsDebugData _graphicsDebugData;
	private:
		//SceneSystem���� Add/Object ������ �ʰ� �ϱ� ���ؼ�.
		std::vector<std::string> _addObjectPlanList;
		std::vector<std::string> _deleteObjectPlanList;

		void HandleAddDeleteInScene();

		
	};

	template <typename T>
	T* Pg::Data::Scene::FindSingleComponentInScene()
	{
		T* tRet = nullptr;
		for (auto& it : this->GetObjectList())
		{
			tRet = it->GetComponent<T>();
			if (tRet != nullptr)
			{
				break;
			}
		}

		return tRet;
	}

}


