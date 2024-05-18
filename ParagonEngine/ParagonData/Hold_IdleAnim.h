#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>

#include "../ParagonData/BtNodes/BasePgBtNode.h"

namespace Pg::Util::Time { class TimeSystem; }

namespace Pg::Data::BTree::Node
{
    class Hold_IdleAnim : public BT::SyncActionNode, public BasePgBtNode
    {
    public:
        Hold_IdleAnim(const std::string& name, const BT::NodeConfiguration& config);
        virtual ~Hold_IdleAnim() = default;

        virtual BT::NodeStatus tick() override;

        static BT::PortsList providedPorts()
        {
            return {};
        }

    private:
        Pg::Util::Time::TimeSystem* _deltaTime;
        float _value;
        bool _isReturn;
    };
}