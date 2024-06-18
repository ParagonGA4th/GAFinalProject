#include "isNeutralize.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isNeutralize::InitCustom()
	{
		config().blackboard->set<bool>("NETINIT", false);
	}

	BT::NodeStatus isNeutralize::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->isNeutralize)
			{
				config().blackboard->set<bool>("NETINIT", true);
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