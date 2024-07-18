#pragma once
#include "Transform.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"
#include <unordered_map>
#include <cassert>

/// <summary>
/// 게임오브젝트 클래스. 
/// 여기가 중요한데... 천천히 공부해서 추가하자!
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
		//게임 오브젝트는 기본적으로 생성 시 무조건 이름을 갖는다.
		GameObject(const std::string name, Scene* belongScene = nullptr);
		virtual ~GameObject();

	public:

		//게임 로직 스크립트들이 사용해서는 안된다.
		void Internal_EngineAwake();
		void BeforePhysicsAwake();
		void Awake();
		void Start();

		//게임 로직 스크립트들이 사용해서는 안된다.
		void Internal_EngineUpdate();
		void BeforePhysicsUpdate(); //Physics System이 호출되기 전에 업데이트를 실행해야 할 때. 
		void Update();
		void FixedUpdate();
		void LateUpdate();

		//EditMode로 변경되었을 때, 정보를 리셋할 수 있게 해주는 애. (Static Scene List 전용)
		void OnEngineStop();

		//충돌에 관한 함수.
		void OnCollisionStay();
		void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count);
		void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count);

		//트리거에 관한 함수.
		void OnTriggerStay();
		void OnTriggerEnter(Collider** _colArr, unsigned int count);
		void OnTriggerExit(Collider** _colArr, unsigned int count);

		//애니메이션 끝난거 신호.
		void OnAnimationEnd();

		//일반 오브젝트 / 컴포넌트들에 한해 영향이 없을 것이다.
		//DontDestroyOnLoad로 마킹된 애들만,
		//해당 함수가 Scene이 변경되었을 때 실행된다.
		void OnSceneChange_Global(Pg::Data::Scene* changedScene);

		//Update Loop이랑 별개로 동작, GrabManagedObjects (매니저의 관리를 위해)
		//매니저들은 일반적으로 [SerializeField] 등으로 가져올 요소들을
		//해당 함수에서 가지고 있어야 한다.
		//그래야 Project 단위의 작업이 가능할 것이다.
		//Tag / Name으로 가져오는 작업만 해당 공간에서 할 수 있다.
		void GrabManagedObjects();

		//Don't Destroy가 아닌 오브젝트들 한정, Effect Object 등 Update와 독립적인 오브젝트들 있을 경우
		//Manual Cleanup 허용하기.
		void CleanOnSceneChange();

	public:
		void OnDestroy();

		const std::string& GetName() const;
		void SetName(const std::string& name);

		void SetActive(bool active);
		bool GetActive();

		const std::string& GetTag() const;
		void SetTag(const std::string& tag);

		//True면 씬 이동할 때 파괴되지 않도록 할 것.
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

		//오브젝트가 가지고 있는 컴포넌트 리스트를 Get.
		template<typename T>
		std::vector<T*> GetComponents();

		//오브젝트가 갖고 있는 특정 컴포넌트들의 개수를 구한다.
		template<typename T>
		unsigned int GetComponentCount();

		//렌더러 호환을 위해, ComponentList 자체 반환.
		//std::unordered_map<std::string, Component*>& GetComponentList();
		std::vector<std::pair<std::string, Component*>>& GetComponentList();

		//오브젝트 본인이 속한 Scene 접속.
		Pg::Data::Scene* GetScene();

		//Awake / Start 함수 다시금 실행 할 수 있게 만들어놓는다.
		void ResetDebouncerBoolean();

		void TurnOnAnimationEnd(const std::string& justEndedAnimation);
	public:
		Transform& _transform;

	private:
	//이제 오브젝트 단위의 isStarted / isAwake 역시 가능.
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

		//OnAnimationEnd 기록용.
		std::string _tempRecordJustEndedAnimation;

	private:
		Pg::Data::Scene* _belongScene;

	private:
		//컴포넌트의 이름과 주소를 저장해놓는 리스트.
		//std::unordered_map<std::string, Component*> _componentList;
		std::vector<std::pair<std::string, Component*>> _componentList;
	};

	///템플릿을 활용한 GetComponent/AddComponent.
	///지금은 아는 방식이 이것뿐이라 PT까지는 이 방식으로 가져가되
	/// 추후 더 공부해서 발전시킬 예정
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

	///컴포넌트를 제거하는 템플릿.
	/// typeid를 활용해서 리스트에 있는 컴포넌트를 erase한다.
	/// 2024.01.31
	template<typename T>
	bool GameObject::RemoveComponent()
	{
		////리스트를 쭉 돌아서 해당 값이 존재하면 지운다.
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

	///따로 추가한 GetComponents.
	///Collider 부분에서 모든 종류의 Collider를 그릴 때
	/// 컴포넌트를 전부 가져와야하기 때문. 근데 아마 다른 곳에도 필요할 수도?
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

