#include "isCoolDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isCoolDown::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_stubFlag._stubState == Pg::Data::StubState::SKILL_COOLDOWN || 
				monHelper->_stubFlag._stubState == Pg::Data::StubState::BASICATTACK_COOLDOWN)
			{
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}