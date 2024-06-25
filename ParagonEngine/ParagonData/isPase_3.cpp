#include "isPase_3.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isPase_3::InitCustom()
	{
		config().blackboard->set<bool>("PASETHREEINIT", false);
	}

	BT::NodeStatus isPase_3::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			bool paseInit = config().blackboard->get<bool>("PASETHREEINIT");
			if (monHelper->_isPase_3)
			{
				monHelper->_bossPase = Pg::Data::BossPase::PASE_3;
				if (!paseInit)
				{
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_1;
					config().blackboard->set<bool>("PASETHREEINIT", true);
				}
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				if (paseInit) config().blackboard->set<bool>("PASETHREEINIT", false);
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}