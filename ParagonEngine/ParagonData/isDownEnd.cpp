#include "isDownEnd.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isDownEnd::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._isDownEnd) return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
}