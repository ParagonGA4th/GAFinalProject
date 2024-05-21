#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>

#include "BtNodes/BasePgBtNode.h"

namespace Pg::Util::Time { class TimeSystem; }

namespace Pg::Data::BTree::Node
{
	class DefaultAnim_Delay : public BT::SyncActionNode, public BasePgBtNode
	{
	public:
		DefaultAnim_Delay(const std::string& name, const BT::NodeConfiguration& config)
			:BT::SyncActionNode(name, config) {};
		virtual ~DefaultAnim_Delay() = default;

		virtual void InitCustom();

		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts()
		{
			BT::PortsList list;
			list.insert(BT::BidirectionalPort<float>("_deHoldTime"));
			list.insert(BT::BidirectionalPort<bool>("_isDeAnimChange"));
			list.insert(BT::BidirectionalPort<std::string>("_deCurrentAnim"));

			return list;
		}

	private:
		Pg::Util::Time::TimeSystem* _deltaTime;
		float _value;
	};
}

