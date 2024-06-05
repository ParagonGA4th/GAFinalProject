#ifndef ISPLAYERINSIGHT
#define ISPLAYERINSIGHT

#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isPlayerInSight : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isPlayerInSight(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~isPlayerInSight() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif