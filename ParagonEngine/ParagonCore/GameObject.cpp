#include "GameObject.h"

namespace Pg::Core
{
	GameObject::GameObject(const std::string& name) :
		_objName(name),
		_isActive(true)
	{

	}

	GameObject::~GameObject()
	{

	}

	void GameObject::Awake()
	{

	}

	void GameObject::Start()
	{

	}

	void GameObject::Update()
	{

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

	}

}
