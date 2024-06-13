#include "isCoolDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isCoolDown::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerinHitSpace && 
				(monHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK || monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_3))
			{
				if(monHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK) monHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN;
				if(monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_3) monHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN;

				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}