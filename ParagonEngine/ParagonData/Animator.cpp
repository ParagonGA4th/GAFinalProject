#include "Animator.h"

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner)
	{
		//역참조자로 값 전달하므로, 명시적으로 Heap에 있기는 해야 한다.
		_behavTree = new BT::Tree();
	}

	Animator::~Animator()
	{
		if (_behavTree != nullptr)
		{
			delete _behavTree;
		}
	}

	void Animator::SetBehaviorTreePath(const std::string& path)
	{
		_behaviorTreePath = path;
	}

}