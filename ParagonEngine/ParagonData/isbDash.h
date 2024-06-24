#pragma once
class isbDash
{
};

#ifndef ISBDASH 
#define ISBDASH
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isbDash : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isbDash(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~isbDash() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif