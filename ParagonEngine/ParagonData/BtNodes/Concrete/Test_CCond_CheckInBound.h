#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>
#include "../BasePgBtNode.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// (테스트) 영역 안에 있는지 확인 (X,Y) 좌표만.
/// </summary>
/// 

namespace Pg::Data::BTree::Node
{
	class Test_CCond_CheckInBound : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		//BehaviorTree 필수 요구 사항.
		Test_CCond_CheckInBound(const std::string& name, const BT::NodeConfiguration& config) :
			BT::SyncActionNode(name, config) {}
		virtual ~Test_CCond_CheckInBound() = default;

		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			BT::PortsList tRet;
			tRet.insert(BT::InputPort<Pg::Math::PGFLOAT2>("_LT")); // {30,30} 로 Groot에서 세팅하자.
			tRet.insert(BT::InputPort<Pg::Math::PGFLOAT2>("_RB")); // {50,50} 로 Groot에서 세팅하자.
			tRet.insert(BT::InputPort<Pg::Math::PGFLOAT3>("_currentPos"));
			return tRet;
		}
	};
}
