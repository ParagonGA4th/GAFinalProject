#include "isHitSpace.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isHitSpace::tick()
	{
		//auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		//if (monHelper != nullptr)
		//{
		//	if (monHelper->_isPlayerDetected)
		//	{
		//		return BT::NodeStatus::SUCCESS;
		//	}
		//	else
		//	{
		//		return BT::NodeStatus::FAILURE;
		//	}
		//}

		return BT::NodeStatus::SUCCESS;
	}
}