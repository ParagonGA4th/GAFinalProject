#include "isChase.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isChase::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isChase)
			{
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}