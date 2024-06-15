#include "isCoolDown.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void isCoolDown::InitCustom()
	{
		config().blackboard->set<bool>("ISCOOLDOWNANIMEND", false);
	}

	BT::NodeStatus isCoolDown::tick()
	{
		bool isCoolDownAnimEnd = config().blackboard->get<bool>("ISCOOLDOWNANIMEND");
		bool isSkillAnimEnd = config().blackboard->get<bool>("ISSKILLANIMEND");

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isPlayerinHitSpace && isSkillAnimEnd && monHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK)
			{
				monHelper->_trentState = Pg::Data::TrentState::SKILL_COOLDOWN;
				return BT::NodeStatus::SUCCESS;
			}
			else if (monHelper->_isPlayerinHitSpace && monHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN)
			{
				return BT::NodeStatus::SUCCESS;
			}
			else if (!isCoolDownAnimEnd &&
				(monHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN || monHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN))
			{
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}