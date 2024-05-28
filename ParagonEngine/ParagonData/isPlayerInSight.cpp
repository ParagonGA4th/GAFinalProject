#include "isPlayerInSight.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isPlayerInSight::InitCustom()
	{
		config().blackboard->set<bool>("ISFINDPLAYER", false);
	}

	BT::NodeStatus isPlayerInSight::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerDetected)
			{
				config().blackboard->set<bool>("ISFINDPLAYER", true);
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