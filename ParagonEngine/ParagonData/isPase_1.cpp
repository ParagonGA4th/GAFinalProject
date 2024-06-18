#include "isPase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPase_1::tick()
	{
		//auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		//if (monHelper != nullptr)
		//{
			//if (monHelper->isPase_1)
			//{
			// monHelper->_bossState = Pg::Data::BossState::PASE_1;
			//	return BT::NodeStatus::SUCCESS;
			//}
			//else
			//{
			//	config().blackboard->set<std::string>("PREVANIM", "");
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

			//	return BT::NodeStatus::FAILURE;
			//}
		//}


		return BT::NodeStatus::FAILURE;
	}
}