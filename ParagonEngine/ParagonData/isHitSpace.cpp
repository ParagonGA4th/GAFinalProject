#include "isHitSpace.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isHitSpace::InitCustom()
	{
		config().blackboard->set<bool>("HITINIT", false);
	}

	BT::NodeStatus isHitSpace::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerinHitSpace)
			{
				config().blackboard->set<bool>("HITINIT", true);
				return BT::NodeStatus::SUCCESS;
			}
			else  
			{
				config().blackboard->set<bool>("HITINIT", false);
				config().blackboard->set<std::string>("PREVANIM", "");

				return BT::NodeStatus::FAILURE;
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}