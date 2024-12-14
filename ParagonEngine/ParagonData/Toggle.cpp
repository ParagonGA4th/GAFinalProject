#include "Toggle.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Toggle::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (!isHolding)
		{
			_toggle = !_toggle;
		}

		if (_toggle)
			return BT::NodeStatus::SUCCESS;
		else
			return BT::NodeStatus::FAILURE;
	}
}