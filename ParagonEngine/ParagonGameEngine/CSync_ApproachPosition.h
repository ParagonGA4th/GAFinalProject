#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>
#include "BasePgBtNode.h"

/// <summary>
/// BT::SyncActionNode
/// 커스텀 조절할 수 있는 싱글스레드 노드. 개별적으로 구현체가 있어야 한다.
/// Single 쓰레드 기반, Leaf Node의 예시.
/// 무조건 tick(), 그리고 인풋/아웃풋 포트가 있을 경우 정적 providedPorts()가 있어야 한다.
/// 모든 Custom Node들은 무조건 PgBtNode를 받아야 한다. 
/// (그래야 GetSharedData()와 그래야 만약 Instanced인 경우 GetObject()를 쓸 수 있음)
/// System 딴에서 추가적으로 Custom Initialize해서 멤버 변수 등을 셋업하고 싶다면, 해당 함수 호출.
///	virtual void InitCustom() override;
/// </summary>

/// <summary>
/// BT::SimpleActionNode
/// SyncActionNode를 더 쉽게 사용할 수 있게 만들어진 SimpleActionNode를 가지고 연산할 수도 있다. 
/// 유저는 다만 BT::NodeStatus functionName(TreeNode&) 시그니처의 콜백함수를 제공해줘야 한다.
/// 이 경우, 개별적으로 클래스를 만들 필요가 없다.
/// </summary>

namespace Pg::Engine::BTree::Node
{
	class CSync_ApproachPosition : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		//BehaviorTree 필수 요구 사항.
		CSync_ApproachPosition(const std::string& name, const BT::NodeConfiguration& config) : 
			BT::SyncActionNode(name, config) {}
		virtual ~CSync_ApproachPosition() = default;
		
		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//System 딴에서 추가적으로 Custom Initialize해서 멤버 변수 등을 셋업하고 싶다면, 해당 함수 호출.
		virtual void InitCustom() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			return { BT::InputPort<float>("_xGoalPos") };
		}
	};
}


