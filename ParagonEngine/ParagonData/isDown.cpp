#include "isDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isDown::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._isDown)
			{
				monHelper->_bossFlag._bossState = Pg::Data::BossState::DOWN;
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				monHelper->_bossFlag._bossState = Pg::Data::BossState::DOWNENDED;
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}