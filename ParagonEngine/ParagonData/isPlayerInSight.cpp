#include "isPlayerInSight.h"
#include "../ParagonUtil/Log.h"


namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPlayerInSight::tick()
	{
		PG_TRACE("PlayerInSIght");
		return BT::NodeStatus::SUCCESS;
	}
}
