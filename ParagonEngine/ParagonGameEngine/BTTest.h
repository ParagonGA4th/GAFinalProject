#pragma once
#include "behaviortree_cpp_v3/bt_factory.h"

class BTTest
{
	BTTest()
	{
		BT::BehaviorTreeFactory factory;
		BT::Tree t = factory.createTree("adsk");
	}
};

