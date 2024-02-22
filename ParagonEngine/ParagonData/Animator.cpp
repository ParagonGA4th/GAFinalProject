#include "Animator.h"

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::SetBehaviorTreePath(const std::string& path)
	{
		_behaviorTreePath = path;
	}

}