#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>

/// <summary>
/// Single 쓰레드 기반, Leaf Node의 예시. 
/// </summary>

namespace Pg::Engine::BTNode
{
	class CSync_ApproachPosition : public BT::SyncActionNode
	{
	public:

		//BehaviorTree 필수 요구 사항.
		CSync_ApproachPosition(const std::string& name, const BT::NodeConfiguration& config) : 
			BT::SyncActionNode(name, config) {}
		virtual ~CSync_ApproachPosition();
		
		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		static BT::PortsList providedPorts()
		{
			return { BT::InputPort<std::string>("P_PlayerInfo") };
		}
	};
}


