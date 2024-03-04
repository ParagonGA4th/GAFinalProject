#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>
#include "BasePgBtNode.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class DynamicCollider;
}

namespace Pg::Engine::BTree::Node
{
	class Test_CSync_JumpAtBumpLimit : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		//BehaviorTree 필수 요구 사항.
		Test_CSync_JumpAtBumpLimit(const std::string& name, const BT::NodeConfiguration& config) :
			BT::SyncActionNode(name, config) {}
		virtual ~Test_CSync_JumpAtBumpLimit() = default;

		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//System 딴에서 추가적으로 Custom Initialize해서 멤버 변수 등을 셋업하고 싶다면, 해당 함수 호출.
		virtual void InitCustom() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			BT::PortsList tRet;
			tRet.insert(BT::InputPort<int>("_jumpCount"));
			return tRet;
		}

	private:
		Pg::Data::DynamicCollider* _collider = nullptr;
	};
}




