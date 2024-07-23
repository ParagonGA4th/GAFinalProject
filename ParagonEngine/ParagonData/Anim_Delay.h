#ifndef ANIM_DELAY
#define ANIM_DELAY
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Util::Time { class TimeSystem; }

namespace Pg::Data::BTree::Node
{
	class Anim_Delay : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_Delay(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~Anim_Delay() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<float>("_holdTime"));
			list.insert(BT::BidirectionalPort<bool>("_isHolding"));
			return list;
		}

	private:
		Pg::Util::Time::TimeSystem* _deltaTime;
		float _value{ 0.f };
	};
}


#endif