#ifndef ANIM_STAB 
#define ANIM_STAB
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_Stab : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_Stab(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_Stab() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}

	private:
		bool _isAnimEnd = { false };
		bool _isChangeAnim = { false };
	};
}
#endif