#pragma once
#include "Transform.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"
#include <unordered_map>
#include <cassert>

/// <summary>
/// ���ӿ�����Ʈ Ŭ����. 
/// ���Ⱑ �߿��ѵ�... õõ�� �����ؼ� �߰�����!
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Data
{
	class GameObject
	{
	public:
		//���� ������Ʈ�� �⺻������ ���� �� ������ �̸��� ���´�.
		GameObject(const std::string name, Scene* belongScene = nullptr);
		virtual ~GameObject();

	public:

		//���� ���� ��ũ��Ʈ���� ����ؼ��� �ȵȴ�.
		void Internal_EngineAwake();
		void BeforePhysicsAwake();
		void Awake();
		void Start();

		//���� ���� ��ũ��Ʈ���� ����ؼ��� �ȵȴ�.
		void Internal_EngineUpdate();
		void BeforePhysicsUpdate(); //Physics System�� ȣ��Ǳ� ���� ������Ʈ�� �����ؾ� �� ��. 
		void Update();
		void FixedUpdate();
		void LateUpdate();

		//EditMode�� ����Ǿ��� ��, ������ ������ �� �ְ� ���ִ� ��. (Static Scene List ����)
		void OnEngineStop();

		//�浹�� ���� �Լ�.
		void OnCollisionStay();
		void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count);
		void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count);

		//Ʈ���ſ� ���� �Լ�.
		void OnTriggerStay();
		void OnTriggerEnter(Collider** _colArr, unsigned int count);
		void OnTriggerExit(Collider** _colArr, unsigned int count);

		//�ִϸ��̼� ������ ��ȣ.
		void OnAnimationEnd();

		//�Ϲ� ������Ʈ / ������Ʈ�鿡 ���� ������ ���� ���̴�.
		//DontDestroyOnLoad�� ��ŷ�� �ֵ鸸,
		//�ش� �Լ��� Scene�� ����Ǿ��� �� ����ȴ�.
		void OnSceneChange_Global(Pg::Data::Scene* changedScene);

		//Update Loop�̶� ������ ����, GrabManagedObjects (�Ŵ����� ������ ����)
		//�Ŵ������� �Ϲ������� [SerializeField] ������ ������ ��ҵ���
		//�ش� �Լ����� ������ �־�� �Ѵ�.
		//�׷��� Project ������ �۾��� ������ ���̴�.
		//Tag / Name���� �������� �۾��� �ش� �������� �� �� �ִ�.
		void GrabManagedObjects();

		//Don't Destroy�� �ƴ� ������Ʈ�� ����, Effect Object �� Update�� �������� ������Ʈ�� ���� ���
		//Manual Cleanup ����ϱ�.
		void CleanOnSceneChange();

	public:
		void OnDestroy();

		const std::string& GetName() const;
		void SetName(const std::string& name);

		void SetActive(bool active);
		bool GetActive();

		const std::string& GetTag() const;
		void SetTag(const std::string& tag);

		//True�� �� �̵��� �� �ı����� �ʵ��� �� ��.
		void SetDontDestroyOnLoad(bool val);
		bool GetDontDestroyOnLoad();

	public:
		template<typename T>
		T* AddComponent();
		Pg::Data::Component* AddComponent(std::string componentType);
		void AddComponent(std::string componentType, Pg::Data::Component* component);

		template<typename T>
		T* GetComponent();

		template<typename T>
		bool RemoveComponent();
		bool RemoveComponent(std::string componentType);

		//������Ʈ�� ������ �ִ� ������Ʈ ����Ʈ�� Get.
		template<typename T>
		std::vector<T*> GetComponents();

		//������Ʈ�� ���� �ִ� Ư�� ������Ʈ���� ������ ���Ѵ�.
		template<typename T>
		unsigned int GetComponentCount();

		//������ ȣȯ�� ����, ComponentList ��ü ��ȯ.
		//std::unordered_map<std::string, Component*>& GetComponentList();
		std::vector<std::pair<std::string, Component*>>& GetComponentList();

		//������Ʈ ������ ���� Scene ����.
		Pg::Data::Scene* GetScene();

		//Awake / Start �Լ� �ٽñ� ���� �� �� �ְ� �������´�.
		void ResetDebouncerBoolean();

		void TurnOnAnimationEnd(const std::string& justEndedAnimation);
	public:
		Transform& _transform;

	private:
	//���� ������Ʈ ������ isStarted / isAwake ���� ����.
		bool _isInternalEngineAwake{ false };
		bool _isInternalBeforePhysicsAwake{ false };
		bool _isAwake{ false };
		bool _isStarted{ false };
		bool _isTurnedOnAnimationEnd{ false };
		bool _dontDestroyOnLoad{ false };

	private:
		bool _isActive;
		std::string _objName;
		std::string _objTag;

		//OnAnimationEnd ��Ͽ�.
		std::string _tempRecordJustEndedAnimation;

	private:
		Pg::Data::Scene* _belongScene;

	private:
		//������Ʈ�� �̸��� �ּҸ� �����س��� ����Ʈ.
		//std::unordered_map<std::string, Component*> _componentList;
		std::vector<std::pair<std::string, Component*>> _componentList;
	};

	///���ø��� Ȱ���� GetComponent/AddComponent.
	///������ �ƴ� ����� �̰ͻ��̶� PT������ �� ������� ��������
	/// ���� �� �����ؼ� ������ų ����
	/// 2023.09.14
	template<typename T>
	T* GameObject::AddComponent()
	{
		T* component = new T(this);
		_componentList.push_back(std::make_pair(typeid(T).name(), component));
		return component;
	}	

	template<typename T>
	T* GameObject::GetComponent()
	{
		for (auto& iter : _componentList)
		{
			T* res = dynamic_cast<T*>(iter.second);
			auto com1 = typeid(T*).name();
			auto com2 = typeid(res).name();

			if (res && strcmp(com1, com2) == 0)
			{
				return res;
			}
		}
		return nullptr;
	}

	///������Ʈ�� �����ϴ� ���ø�.
	/// typeid�� Ȱ���ؼ� ����Ʈ�� �ִ� ������Ʈ�� erase�Ѵ�.
	/// 2024.01.31
	template<typename T>
	bool GameObject::RemoveComponent()
	{
		////����Ʈ�� �� ���Ƽ� �ش� ���� �����ϸ� �����.
		//auto iter = _componentList.find(typeid(T).name());
		//if (iter != _componentList.end())
		//{
		//	delete iter->second;
		//	_componentList.erase(iter);
		//	return true;
		//}
		//
		//return false;
		std::string componentType = typeid(T).name();


		auto res = std::find_if(_componentList.begin(), _componentList.end(),
			[&componentType](const std::pair<std::string, Component*>& val)
			-> bool {return (val.first == componentType); });

		if (res != _componentList.end())
		{
			delete res->second;
			_componentList.erase(std::remove_if(_componentList.begin(), _componentList.end(), [&componentType](const std::pair<std::string, Component*>& val)
				-> bool {return (val.first == componentType); }),
				_componentList.end());
			return true;
		}

		return false;
	}

	///���� �߰��� GetComponents.
	///Collider �κп��� ��� ������ Collider�� �׸� ��
	/// ������Ʈ�� ���� �����;��ϱ� ����. �ٵ� �Ƹ� �ٸ� ������ �ʿ��� ����?
	/// 2023.10.31
	template<typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> res;
		T* tmp;

		///Structured Binding
		for (const auto& [typeName, component] : _componentList)
		{
			tmp = dynamic_cast<T*>(component);

			if (tmp)
			{
				res.push_back(tmp);
			}
		}

		return res;
	}

	template<typename T>
	unsigned int Pg::Data::GameObject::GetComponentCount()
	{
		T* tmp;
		unsigned int tCount = 0;

		///Structured Binding
		for (const auto& [typeName, component] : _componentList)
		{
			tmp = dynamic_cast<T*>(component);

			if (tmp)
			{
				tCount++;
			}
		}

		return tCount;
	}
}

