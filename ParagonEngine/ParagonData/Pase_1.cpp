#include "Pase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Pase_1::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			//if (monHelper->_Pase_1)
			//{
			//	config().blackboard->set<bool>("HITINIT", true);
			//	return BT::NodeStatus::SUCCESS;
			//}
			//else
			//{
			//	config().blackboard->set<bool>("HITINIT", false);
			//	config().blackboard->set<std::string>("PREVANIM", "");

			//	return BT::NodeStatus::FAILURE;
			//}
		}
		bool attack_1 = config().blackboard->get<bool>("ISRANIMEND");
		bool attack_2 = config().blackboard->get<bool>("ISLANIMEND");
		bool attack_3 = config().blackboard->get<bool>("ISTANIMEND");
		bool hold = config().blackboard->get<bool>("ISCHANGE");;

		if (attack_1 && attack_2 && attack_3 && !hold)
		{
			config().blackboard->set<bool>("ISRANIMEND", false);
			config().blackboard->set<bool>("ISLANIMEND", false);
			config().blackboard->set<bool>("ISTANIMEND", false);
		}

		return BT::NodeStatus::SUCCESS;
	}
}