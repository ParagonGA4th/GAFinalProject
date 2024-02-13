#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>

//https://www.behaviortree.dev/docs/4.0.2/tutorial-advanced/asynchronous_nodes/
//https://www.behaviortree.dev/docs/4.0.2/tutorial-basics/tutorial_04_sequence

/// <summary>
/// 코루틴 등에 유용한 Async Leaf Node의 예시. (StatefulActionNode)
/// </summary>

namespace Pg::Engine::BTNode
{
	class CAsync_Sleeper : public BT::StatefulActionNode
	{
		//생성자.
		CAsync_Sleeper(const std::string& name, const BT::NodeConfiguration& config)
			: BT::StatefulActionNode(name, config)
		{}
		virtual ~CAsync_Sleeper();


		//basictypes.h에 진실.
		//static BT::PortsList providedPorts()
		//{
		//	return{ BT::BidirectionalPort<Pose2D>("goal") };
		//}

	};
}

