#ifndef ANIM_BIDLE
#define ANIM_BIDLE
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Util::Time { class TimeSystem; }

namespace Pg::Data::BTree::Node
{
	class Anim_bIdle : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		Anim_bIdle(const std::string& name, const BT::NodeConfiguration& config)
			: BT::SyncActionNode(name, config) { }
		virtual ~Anim_bIdle() = default;

		virtual BT::NodeStatus tick() override;

		virtual void InitCustom();

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<float>("_holdTime"));
			list.insert(BT::BidirectionalPort<std::string>("_nextAnim"));

			return list;
		}

	private:
		Pg::Util::Time::TimeSystem* _deltaTime;
		float _value = 0.f;
	};
}
#endif