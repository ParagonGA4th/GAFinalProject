#ifndef ANIM_BIDLE
#define ANIM_BIDLE
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_bIdle : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_bIdle(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) { }
		virtual ~Anim_bIdle() = default;

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			return {};
		}

	private:
		bool _isSPSkillTime{ false };
		int _isTPSkillTime = 0;
	};
}
#endif