#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include "BTDefines.h"

namespace Pg::Engine::BTree::Node
{
	class Test_CSync_MoveToNewDir : public BT::SyncActionNode
	{
	public:
		//BehaviorTree 필수 요구 사항.
		Test_CSync_MoveToNewDir(const std::string& name, const BT::NodeConfiguration& config) :
			BT::SyncActionNode(name, config) {}
		virtual ~Test_CSync_MoveToNewDir() = default;

		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			BT::PortsList tRet;
			tRet.insert(BT::InputPort<Pg::Data::GameObject*>(Pg::Engine::BTree::PRIVATE_OBJECT_KEY));
			tRet.insert(BT::InputPort<Pg::Math::PGQuaternion>("_moveDir"));
			return tRet;
		}
	};
}

