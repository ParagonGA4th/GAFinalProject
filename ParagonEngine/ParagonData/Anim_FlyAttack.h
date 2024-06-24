#ifndef ANIM_FLYATTACK 
#define ANIM_FLYATTACK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_FlyAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_FlyAttack(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_FlyAttack() = default;


		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif