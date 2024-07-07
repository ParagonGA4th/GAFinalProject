#include "isgSkill.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isgSkill::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._isSkill)
				return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
}