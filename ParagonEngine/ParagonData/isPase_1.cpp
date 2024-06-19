#include "isPase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isPase_1::InitCustom()
	{
		config().blackboard->set<bool>("PASEINIT", false);
	}

	BT::NodeStatus isPase_1::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			bool paseInit = config().blackboard->get<bool>("PASEINIT");
			if (monHelper->isPase_1)
			{
				monHelper->_bossPase = Pg::Data::BossPase::PASE_1;
				if (!paseInit)
				{
					monHelper->_bossState = Pg::Data::BossState::BASIC_ATTACK_1;
					config().blackboard->set<bool>("PASEINIT", true);
				}
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				if (paseInit) config().blackboard->set<bool>("PASEINIT", false);
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}