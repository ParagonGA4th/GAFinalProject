#include "isPase_2.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isPase_2::InitCustom()
	{
		config().blackboard->set<bool>("PASETWOINIT", false);
	}

	BT::NodeStatus isPase_2::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			bool paseInit = config().blackboard->get<bool>("PASETWOINIT");
			if (monHelper->_isPase_2)
			{
				monHelper->_bossPase = Pg::Data::BossPase::PASE_2;
				if (!paseInit)
				{
					monHelper->_bossState = Pg::Data::BossState::EVASION;
					config().blackboard->set<bool>("PASETWOINIT", true);
				}
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				if (paseInit) config().blackboard->set<bool>("PASETWOINIT", false);
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}