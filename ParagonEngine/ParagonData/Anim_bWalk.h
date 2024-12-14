#ifndef ANIM_BWALK
#define ANIM_BWALK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_bWalk : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_bWalk(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_bWalk() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}

	private:
	};
}
#endif