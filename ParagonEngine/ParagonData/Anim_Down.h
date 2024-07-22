#ifndef ANIM_DOWN
#define ANIM_DOWN
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_Down : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_Down(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_Down() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif

