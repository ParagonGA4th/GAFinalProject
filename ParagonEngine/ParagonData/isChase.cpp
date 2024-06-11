#include "isChase.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isChase::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isChase && !monHelper->_isPlayerinHitSpace)
			{
				config().blackboard->set<bool>("ISCHANGE", false);
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