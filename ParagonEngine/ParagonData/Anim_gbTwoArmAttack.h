#ifndef ANIM_GBTWOARMATTACK 
#define ANIM_GBTWOARMATTACK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_gbTwoArmAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_gbTwoArmAttack(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_gbTwoArmAttack() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	private:
		bool _isCast{ false };
	};
}
#endif