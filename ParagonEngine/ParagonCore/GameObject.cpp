#include "GameObject.h"
#include <algorithm>
namespace Pg::Core
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
		//게임 오브젝트가 소멸 시 컴포넌트도 모두 삭제된다.
		std::for_each(_componentList.begin(), _componentList.end(), [](auto& iter)
			{ delete iter.second; });
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

	void GameObject::OnCollisionStay()
	{

	}

	void GameObject::OnCollisionEnter()
	{

	}

	void GameObject::OnCollisionExit()
	{

	}

	void GameObject::OnDestroy()
	{

	}

}
