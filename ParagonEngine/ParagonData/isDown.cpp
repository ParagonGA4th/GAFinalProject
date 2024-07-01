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
		bool downInit = config().blackboard->get<bool>("DOWNINIT");

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._isDown)
			{
				monHelper->_bossFlag._bossState = Pg::Data::BossState::DOWN;
				config().blackboard->set<bool>("DOWNINIT", true);
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				if (downInit)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::DOWNENDED;
					if (monHelper->_isAnimationEnd)
					{
						config().blackboard->set<bool>("DOWNINIT", false);
						monHelper->_isAnimationEnd = false;
					}
				}
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}