#pragma once
#include "Transform.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <singleton-cpp/singleton.h>

/// <summary>
/// 게임오브젝트 클래스. 
/// 여기가 중요한데... 천천히 공부해서 추가하자!
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class IComponent;
}

namespace Pg::Data
{
	class GameObject
	{
	public:
		//게임 오브젝트는 기본적으로 생성 시 무조건 이름을 갖는다.
		GameObject(const std::string name);
		virtual ~GameObject();

		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();

		//충돌에 관한 함수.
		void OnCollisionStay();
		void OnCollisionEnter();
		void OnCollisionExit();
		void OnDestroy();

		const std::string& GetName() const;
		void SetName(const std::string& name);

		void SetActive(bool active);
		bool GetActive();

	public:
		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent();

		//오브젝트가 가지고 있는 컴포넌트 리스트를 Get.
		template<typename T>
		std::vector<T*> GetComponents();

		//렌더러 호환을 위해, ComponentList 자체 반환.
		std::unordered_map<std::string, Component*>& GetComponentList();
	public:
		Transform& _transform;
	private:
		bool _isActive;
		std::string _objName;

	private:
		//컴포넌트의 이름과 주소를 저장해놓는 리스트.
		std::unordered_map<std::string, Component*> _componentList;
	};

	///템플릿을 활용한 GetComponent/AddComponent.
	///지금은 아는 방식이 이것뿐이라 PT까지는 이 방식으로 가져가되
	/// 추후 더 공부해서 발전시킬 예정
	/// 2023.09.14
	template<typename T>
	T* GameObject::AddComponent()
	{
		T* component = new T(this);
		_componentList.try_emplace(typeid(T).name(), component);
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

	///따로 추가한 GetComponents.
	///Collider 부분에서 모든 종류의 Collider를 그릴 때
	/// 컴포넌트를 전부 가져와야하기 때문. 근데 아마 다른 곳에도 필요할 수도?
	/// 2023.10.31
	template<typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> res;
		T* tmp;

		//for (const auto& com : _componentList)
		//{
		//	tmp = dynamic_cast<T*>(com.second);
		//
		//	if (tmp)
		//	{
		//		res.push_back(tmp);
		//	}
		//}

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
}

