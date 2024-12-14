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

		// ������ �ش� Function�� �������̵� �ؾ� �Ѵ�.
		virtual BT::NodeStatus tick() override;

		//Port�� �ִ� Node�� ������ �ش� ���� �޼��尡 �����Ǿ� �־�� �Ѵ�.
		//Port ������ �׳� {} ��ȯ�ϴ°� Good Practice.
		static BT::PortsList providedPorts()
		{
			return {};
		}
	};
}
#endif