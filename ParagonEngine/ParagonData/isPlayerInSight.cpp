#include "isPlayerInSight.h"
#include "MonsterHelper.h"
#include "../ParagonUtil/Log.h"
#include <string>

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
					return BT::NodeStatus::FAILURE;
				}
				else
				{
					return BT::NodeStatus::SUCCESS;
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}
