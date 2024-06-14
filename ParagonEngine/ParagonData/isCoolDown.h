#ifndef ISCOOLDOWN
#define ISCOOLDOWN

#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isCoolDown : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isCoolDown(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~isCoolDown() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<bool>("_isCoolDownAnimEnd"));

			return list;
		}
	};
}
#endif