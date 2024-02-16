#include "Animator.h"

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::SetBehaviorTree(BT::Tree* behaviorTree)
	{
		_behavTree = behaviorTree;
	}

}