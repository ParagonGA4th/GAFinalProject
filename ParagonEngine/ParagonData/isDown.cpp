#include "isDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isDown::InitCustom()
	{
		config().blackboard->set<bool>("DOWNINIT", false);
	}

	BT::NodeStatus isDown::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isDown)
			{
				config().blackboard->set<bool>("DOWNINIT", true);
				monHelper->_bossState = Pg::Data::BossState::DOWN;
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}