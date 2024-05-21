#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_Rush : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_Rush(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~Anim_Rush() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}

