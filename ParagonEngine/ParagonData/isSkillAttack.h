#ifndef ISSKILLATTACK
#define ISSKILLATTACK

#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class isSkillAttack : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		isSkillAttack(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~isSkillAttack() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif
