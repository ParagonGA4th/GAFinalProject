#include "Animator.h"

#include <generic_factory/generic_factory.hpp>

using namespace Pg::Data;
REGISTER_CHILD_INTO_FACTORY(Component, Animator, "class Pg::Data::Animator", GameObject*);

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner),
		_isUniform(false), _isCulled(false)
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

	void Animator::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void Animator::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

}