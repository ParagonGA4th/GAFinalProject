#include "GameObject.h"

namespace Pg::Core
{
	/// <summary>
	/// 오브젝트가 생성되면 기본적으로 이름과 활성화가 되어있어야 한다!
	/// </summary>
	/// <param name="name"></param>
	GameObject::GameObject(const std::string& name) :
		_objectName(name), _isActive(true)
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

	void GameObject::OnCollisionEnter()
	{

	}

	void GameObject::OnCollisionStay()
	{

	}

	void GameObject::OnCollisionExit()
	{

	}
}
