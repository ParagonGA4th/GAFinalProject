//#pragma once
//#include <behaviortree_cpp_v3/action_node.h>
//#include <behaviortree_cpp_v3/bt_factory.h>
//#include <chrono>
//
////https://www.behaviortree.dev/docs/4.0.2/tutorial-advanced/asynchronous_nodes/
////https://www.behaviortree.dev/docs/4.0.2/tutorial-basics/tutorial_04_sequence
//
///// <summary>
///// 코루틴 등에 유용한 Async Leaf Node의 예시. (StatefulActionNode)
///// </summary>
//
//namespace Pg::Data::BTree::Node
//{
//	class CAsync_Sleeper : public BT::StatefulActionNode
//	{
//	public:
//		//생성자.
//		CAsync_Sleeper(const std::string& name, const BT::NodeConfiguration& config)
//			: BT::StatefulActionNode(name, config) {}
//		virtual ~CAsync_Sleeper() = default;
//
//
//		//Port를 가지고 있는 모든 Node들이 무조건적으로 구현해야 할 정적 메서드.
//		static BT::PortsList providedPorts()
//		{
//			//BT::BidirectionalPort -> ReadWrite일때.
//			//BT::OutputPort -> Write.
//			return { BT::OutputPort<float>("_sleepingLeftSeconds")};
//		}
//
//		//시작할 때 불리는 메서드.
//		//Running을 리턴할 경우, Async Node가 됨.
//		virtual BT::NodeStatus onStart() override;
//
//		// Running 액션에 의해 발동되는 함수.
//		virtual BT::NodeStatus onRunning() override;
//
//		//halt()가 호출되었는데 액션이 Running?
//		//이 메서드가 호출됨. Cleanup이 필요할시 해당 공간이 유용.
//		virtual void onHalted() override;
//
//	};
//}
//
