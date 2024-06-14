#include "isSkillAttack.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isSkillAttack::InitCustom()
	{
		config().blackboard->set<bool>("ISSKILLANIMEND", false);
	}

	BT::NodeStatus isSkillAttack::tick()
	{
		bool isAnimEnd = config().blackboard->get<bool>("ISSKILLANIMEND");

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerinHitSpace &&
				(monHelper->_trentState == Pg::Data::TrentState::IDLE || monHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN))
			{
				if(isAnimEnd) config().blackboard->set<bool>("ISSKILLANIMEND", false);
				monHelper->_trentState = Pg::Data::TrentState::SKILL_ATTACK;
				
				return BT::NodeStatus::SUCCESS;
			}
			else if (monHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK && !isAnimEnd)
			{
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}