#include "goToPlayer.h"

namespace Pg::Data::BTree::Node
{

	void goToPlayer::InitCustom()
	{
		config().blackboard->set<bool>("ANIMEND", false);
	}

	BT::NodeStatus goToPlayer::tick()
	{
		//일단은 빌드시키려고 투입.
		return BT::NodeStatus::SUCCESS;
	}

}
