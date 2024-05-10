#include "isHitSpace.h"
#include "../ParagonData/BtNodes/BTHelper.h"

BT::NodeStatus Pg::Data::BTree::Node::isHitSpace::tick()
{
	auto hitSpace = getInput<bool>("_isHitSpace");
	BT_VALIDATE(hitSpace);

	if (hitSpace)
		return BT::NodeStatus::SUCCESS;
	else
		return BT::NodeStatus::FAILURE;
}
  