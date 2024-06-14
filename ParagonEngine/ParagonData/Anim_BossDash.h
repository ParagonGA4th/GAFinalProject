#ifndef ANIM_BOSSDASH
#define ANIM_BOSSDASH
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_BossDash : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_BossDash(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) {}
		virtual ~Anim_BossDash() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}

	private:
		bool _isAnimEnd = { false };
	};
}
#endif