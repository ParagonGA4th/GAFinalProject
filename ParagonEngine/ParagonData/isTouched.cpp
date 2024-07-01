#include "isTouched.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isTouched::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_mimicFlag._isTouched)
			{
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}