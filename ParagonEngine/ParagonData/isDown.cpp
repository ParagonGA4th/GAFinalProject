#include "isDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isDown::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._isDown) return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
}