#ifndef ISPASE_1
#define ISPASE_1
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isPase_1 : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isPase_1(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~isPase_1() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<bool>("_paseInit"));

			return list;
		}
	};
}
#endif

