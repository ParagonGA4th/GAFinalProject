#ifndef ANIM_LEFTARMATTACK 
#define ANIM_LEFTARMATTACK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_LeftArmAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_LeftArmAttack(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_LeftArmAttack() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<bool>("_isLAnimEnd"));

			return list;
		}
	};
}
#endif