#include "isTouched.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isTouched::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isTouched)
			{
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}

}