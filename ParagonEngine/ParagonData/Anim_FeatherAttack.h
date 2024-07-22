#ifndef ANIM_FEATHERATTACK
#define ANIM_FEATHERATTACK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_FeatherAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_FeatherAttack(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_FeatherAttack() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif

