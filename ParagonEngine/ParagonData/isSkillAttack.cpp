#include "isSkillAttack.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isSkillAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerinHitSpace &&
				(monHelper->_trentState == Pg::Data::TrentState::IDLE || monHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN))
			{
				monHelper->_trentState = Pg::Data::TrentState::SKILL_ATTACK;
				return BT::NodeStatus::SUCCESS;
			}
			else if(monHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK && monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}