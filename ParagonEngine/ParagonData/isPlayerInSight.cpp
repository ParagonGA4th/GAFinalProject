#include "isPlayerInSight.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPlayerInSight::tick()
	{
		auto children = this->GetGameObject()->_transform.GetChildren();
		for (auto& child : children)
		{
			if (child->_object->GetComponent<Pg::Data::MonsterHelper>() != nullptr)
			{
				auto sight = child->_object->GetComponent<Pg::Data::MonsterHelper>();
				if (sight->_isPlayerDetected)
				{
					return BT::NodeStatus::SUCCESS;
				}
				else
				{
					return BT::NodeStatus::FAILURE;
				}
			}
		}
		return BT::NodeStatus::FAILURE;
	}
}