#ifndef ANIM_GIDLE 
#define ANIM_GIDLE
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Data::BTree::Node
{
	class Anim_gIdle : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_gIdle(const std::string& name, const BT::NodeConfiguration& config) :
			BT::SyncActionNode(name, config) {}
		virtual ~Anim_gIdle() = default;

		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif