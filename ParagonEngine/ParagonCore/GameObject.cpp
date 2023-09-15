#include "GameObject.h"


namespace Pg::Core
{
	GameObject::GameObject(const std::string& name) :
		_objName(name),
		_isActive(true),
		_componentList()
	{
		this->AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{

	}

	void GameObject::Awake()
	{

	}

	void GameObject::Start()
	{
		//활성화되지 않으면 시작 안함.
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _componentList)
		{

		}
	}

	void GameObject::Update()
	{
		//활성화되지 않으면 업데이트 안함.
		if (!_isActive)
		{
			return;
		}

	}

	void GameObject::FixedUpdate()
	{

	}

	void GameObject::LateUpdate()
	{

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

}
