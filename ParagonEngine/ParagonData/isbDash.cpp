#include "isbDash.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isbDash::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossState == Pg::Data::BossState::DASH)
				return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
}