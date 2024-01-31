#include "GameObject.h"
#include "PhysicsCollision.h"

#include "RendererBase3D.h"

#include <generic_factory/generic_factory.hpp>
#include <algorithm>

namespace Pg::Data
{
	GameObject::GameObject(const std::string name) :
		_transform(*(new Transform(this))),
		_objName(name),
		_isActive(true),
		_componentList()
	{
		//기본적으로 무조건 GameObject가 생성되면 Transform을 컴포넌트로 갖는다.
		_componentList.insert(std::make_pair(typeid(_transform).name(), &_transform));
	}

	GameObject::~GameObject()
	{
		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ delete iter.second; });
	}

	void GameObject::Awake()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		//for_each구문을 이용하여 componentList를 싹다 돌리기.
		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->Awake(); });
	}

	void GameObject::Start()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		//for_each구문을 이용하여 componentList를 싹다 돌리기.
		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter) 
			{ iter.second->Start(); });
	}

	void GameObject::Update()
	{
		//활성화되지 않으면 업데이트 안함.
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->Update(); });
	}

	void GameObject::FixedUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->FixedUpdate(); });
	}

	void GameObject::LateUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->LateUpdate(); });
	}

	const std::string& GameObject::GetName() const
	{
		return _objName;
	}

	void GameObject::SetName(const std::string& name)
	{
		_objName = name;
	}

	void GameObject::SetActive(bool active)
	{
		if (active != _isActive)
		{
			_isActive = active;
		}
	}

	bool GameObject::GetActive()
	{
		return _isActive;
	}

	const std::string& GameObject::GetTag() const
	{
		return _objTag;
	}

	void GameObject::SetTag(const std::string& tag)
	{
		_objTag = tag;
	}

	Pg::Data::Component* GameObject::AddComponent(std::string componentType)
	{
		Pg::Data::Component* component;

		if (componentType.find("Render") != std::string::npos)
		{
			if (componentType.find("Mesh") != std::string::npos)
			{
				component =
					dynamic_cast<Pg::Data::Component*>(GenericFactory<Pg::Data::RendererBase3D, Pg::Data::GameObject*>::createChild(componentType, this).release());
			}
		}
		else
		{
			component =
			dynamic_cast<Pg::Data::Component*>(GenericFactory<Pg::Data::Component, Pg::Data::GameObject*>::createChild(componentType, this).release());			
		}

		_componentList.try_emplace(componentType, component);

		return component;
	}

	bool GameObject::RemoveComponent(std::string componentType)
	{
		//리스트를 쭉 돌아서 해당 값이 존재하면 지운다.
		auto iter = _componentList.find(componentType);
		if (iter != _componentList.end())
		{
			delete iter->second;
			_componentList.erase(iter);
			return true;
		}

		return false;
	}

	void GameObject::OnCollisionStay()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->OnCollisionStay(); });
	}

	void GameObject::OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count)
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [&_colArr, &count](auto& iter)
			{ iter.second->OnCollisionEnter(_colArr, count); });
	}


	void GameObject::OnCollisionExit(PhysicsCollision** _colArr, unsigned int count)
	{
		if (!_isActive)
		{
			return;
		}
		 
		std::for_each(_componentList.begin(), _componentList.end(), [&_colArr, &count](auto& iter)
			{ iter.second->OnCollisionExit(_colArr, count); });
	}

	void GameObject::OnTriggerStay()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->OnTriggerStay(); });
	}

	void GameObject::OnTriggerEnter()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->OnTriggerEnter(); });
	}

	void GameObject::OnTriggerExit()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->OnTriggerExit(); });
	}


	void GameObject::OnDestroy()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ iter.second->OnDestroy(); });
	}

	std::unordered_map<std::string, Component*>& GameObject::GetComponentList()
	{
		return _componentList;
	}
}
