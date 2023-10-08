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

	public:

		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent();

		//만약 렌더러 중 하나라면, Rendering Logic 연동.
		template<typename T>
		void IfRendererSetup(IComponent* component);

	public:
		std::string _objName;
		Transform& _transform;
	private:
		bool _isActive;

	private:
		//컴포넌트의 이름과 주소를 저장해놓는 리스트.
		std::unordered_map<std::string, IComponent*> _componentList;
	};

	///템플릿을 활용한 GetComponent/AddComponent.
	///지금은 아는 방식이 이것뿐이라 PT까지는 이 방식으로 가져가되
	/// 추후 더 공부해서 발전시킬 예정
	/// 2023.09.14
	template<typename T>
	T* GameObject::AddComponent()
	{
		T* component = new T(this);
		auto tBoolPair = _componentList.try_emplace(typeid(T).name(), component);

		//실제로 추가된 것이라면 렌더러와 연동. (나중에 Component System이 할 일)
		if (tBoolPair.second)
		{
			IfRendererSetup<T>(component);
		}

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

	template<typename T>
	void GameObject::IfRendererSetup(IComponent* component)
	{
		//[TW] Renderer 연동 위한 사용. 후에, 별도의 수단이 마련되어야 한다. / 
		// 나중에 Delete 추가되면 이 역시 업데이트되어야!

		//ComponentList에 정상적으로 추가되었다는 전제, 렌더러-그래픽스 연동 로직.
		//코드가 리팩토링 / Event & Component 시스템이 추가되며, 사라져야 할 코드.
		//이러면 Renderer를 베이스로 하는 컴포넌트 여러 개가 들어가면 안 될것!

		if constexpr (std::is_base_of<BaseRenderer, T>::value)
		{
			BaseRenderer* tBaseRenderer = static_cast<BaseRenderer*>(component);
			tBaseRenderer->SetRendererTypeName(typeid(T).name());
		}
	}
}

