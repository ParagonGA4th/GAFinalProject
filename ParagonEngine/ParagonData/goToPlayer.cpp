#include "goToPlayer.h"

namespace Pg::Data::BTree::Node
{

	void goToPlayer::InitCustom()
	{
		config().blackboard->set<bool>("ANIMEND", false);
	}

	BT::NodeStatus goToPlayer::tick()
	{
		return BT::NodeStatus::SUCCESS;
	}

}
