#ifndef ISPASE_3
#define ISPASE_3
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isPase_3 : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isPase_3(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~isPase_3() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}

	private:
		bool _isInit;
	};
}
#endif

