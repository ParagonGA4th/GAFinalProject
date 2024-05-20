#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>

#include "../ParagonData/BtNodes/BasePgBtNode.h"

namespace Pg::Util::Time { class TimeSystem; }

namespace Pg::Data::BTree::Node
{
	class Hold_DefaultAnim : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Hold_DefaultAnim(const std::string& name, const BT::NodeConfiguration& config);
		virtual ~Hold_DefaultAnim() = default;

		virtual void InitCustom();

		// 무조건 해당 Function을 오버라이드 해야 한다.
		virtual BT::NodeStatus tick() override;

		//Port가 있는 Node는 무조건 해당 정적 메서드가 구현되어 있어야 한다.
		//Port 없으면 그냥 {} 반환하는게 Good Practice.
		static BT::PortsList providedPorts()
		{
			BT::PortsList tRet;
			tRet.insert(BT::BidirectionalPort<float>("_holdTime"));
			tRet.insert(BT::BidirectionalPort<bool>("_isAnimChange"));
			tRet.insert(BT::BidirectionalPort<std::string>("_currentAnim"));
			return tRet;
		}

	private:
		Pg::Util::Time::TimeSystem* _deltaTime;
	};
}