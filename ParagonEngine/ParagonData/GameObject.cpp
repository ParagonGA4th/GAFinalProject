#include "GameObject.h"
#include "Scene.h"
#include "PhysicsCollision.h"

// Component
#include "ComponentList.h"
#include "../ParagonUtil/Log.h"

#include "data_factory.h" // Auto Register를 위한 필수요건.
#include <algorithm>

namespace Pg::Data
{
	GameObject::GameObject(const std::string name, Scene* belongScene) :
		_transform(*(new Transform(this))),
		_objName(name),
		_isActive(true),
		_belongScene(belongScene),
		_componentList(),
		_isAwake(false),
		_isStarted(false)
	{
		//기본적으로 무조건 GameObject가 생성되면 Transform을 컴포넌트로 갖는다.
		//_componentList.insert(std::make_pair(typeid(_transform).name(), &_transform));
		_componentList.push_back(std::make_pair(typeid(_transform).name(), &_transform));
	}

	GameObject::~GameObject()
	{
		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ delete iter.second; });
	}

	void GameObject::Internal_EngineAwake()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		if (!_isInternalEngineAwake)
		{
			//for_each구문을 이용하여 componentList를 싹다 돌리기.
			std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
				{
					if (iter.second->GetActive())
					{
						iter.second->Internal_EngineAwake();
					}
				});

			_isInternalEngineAwake = true;
		}
	}

	void GameObject::Awake()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		if (!_isAwake)
		{
			//for_each구문을 이용하여 componentList를 싹다 돌리기.
			std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
				{
					if (iter.second->GetActive())
					{
						iter.second->Awake();
					}
				});

			_isAwake = true;
		}
	}

	void GameObject::Start()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		if (!_isStarted)
		{
			//for_each구문을 이용하여 componentList를 싹다 돌리기.
			std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
				{
					if (iter.second->GetActive())
					{
						iter.second->Start();
					}
				});

			_isStarted = true;
		}
	}

	void GameObject::Internal_EngineUpdate()
	{
		//활성화되지 않으면 업데이트 안함.
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{
				if (iter.second->GetActive())
				{
					iter.second->Internal_EngineUpdate();
				}
			});
	}

	void GameObject::Update()
	{
		//활성화되지 않으면 업데이트 안함.
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{
				if (iter.second->GetActive())
				{
					iter.second->Update();
				}
			});
	}

	void GameObject::FixedUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{
				if (iter.second->GetActive())
				{
					iter.second->FixedUpdate();
				}
			});
	}

	void GameObject::LateUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{
				if (iter.second->GetActive())
				{
					iter.second->LateUpdate();
				}
			});
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
		component = Pg::Factory::Data::Factory<Pg::Data::Component, Pg::Data::GameObject*>::Create(componentType.c_str(), this);
		if (component != nullptr)
		{
			AddComponent(componentType, component);
			return component;
		}

		///TODO: 현재 Static/SkinnedMeshRenderer가 먹지 않고 있다. UE를 급하게 고쳐야 하기에, 하드코딩으로 일단 투입.
		if (componentType.compare("class Pg::Data::StaticMeshRenderer") == 0)
		{
			component = new StaticMeshRenderer(this);
			AddComponent("class Pg::Data::StaticMeshRenderer", component);
			return component;
		}

		if (componentType.compare("class Pg::Data::SkinnedMeshRenderer") == 0)
		{
			component = new SkinnedMeshRenderer(this);
			AddComponent("class Pg::Data::SkinnedMeshRenderer", component);
			return component;
		}

		//component = Pg::Factory::Data::Factory<Pg::Data::RendererBase3D, Pg::Data::GameObject*>::Create(componentType.c_str(), this);
		//if (component != nullptr)
		//{
		//	AddComponent(componentType, component);
		//	return component;
		//}

		component = Pg::Factory::Data::Factory<Pg::Data::StaticCollider, Pg::Data::GameObject*>::Create(componentType.c_str(), this);
		if (component != nullptr)
		{
			AddComponent(componentType, component);
			return component;
		}

		component = Pg::Factory::Data::Factory<Pg::Data::DynamicCollider, Pg::Data::GameObject*>::Create(componentType.c_str(), this);
		if (component != nullptr)
		{
			AddComponent(componentType, component);
			return component;
		}

		return nullptr;
	}

	void GameObject::AddComponent(std::string componentType, Pg::Data::Component* component)
	{
		_componentList.emplace_back(std::make_pair(componentType, component));
	}

	bool GameObject::RemoveComponent(std::string componentType)
	{
		//리스트를 쭉 돌아서 해당 값이 존재하면 지운다.
		//auto iter = _componentList.find(componentType);

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

	void GameObject::OnTriggerEnter(Collider* c)
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [&c](auto& iter)
			{ iter.second->OnTriggerEnter(c); });
	}

	void GameObject::OnTriggerExit(Collider* c)
	{
		if (!_isActive)
		{
			return;
		}

		std::for_each(_componentList.begin(), _componentList.end(), [&c](auto& iter)
			{ iter.second->OnTriggerExit(c); });
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

	std::vector<std::pair<std::string, Component*>>& GameObject::GetComponentList()
	{
		return _componentList;
	}

	Pg::Data::Scene* GameObject::GetScene()
	{
		assert(_belongScene != nullptr);
		return _belongScene;
	}

	void GameObject::ResetDebouncerBoolean()
	{
		_isAwake = false;
		_isStarted = false;
		_isInternalEngineAwake = false;
	}
}
