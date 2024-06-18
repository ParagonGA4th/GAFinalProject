#ifndef ANIM_RIGHTARMATTACK 
#define ANIM_RIGHTARMATTACK
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_RightArmAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_RightArmAttack(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_RightArmAttack() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<bool>("_isRAnimEnd"));

			return list;
		}
	};
}
#endif