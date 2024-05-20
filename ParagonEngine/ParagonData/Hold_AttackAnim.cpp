#include "Hold_AttackAnim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

#include <string>

namespace Pg::Data::BTree::Node
{
	Hold_AttackAnim::Hold_AttackAnim(const std::string& name, const BT::NodeConfiguration& config)
		: BT::SyncActionNode(name, config)
	{
		auto& tdelta = singleton<Pg::Util::Time::TimeSystem>();
		_deltaTime = &tdelta;
	}

	void Hold_AttackAnim::InitCustom()
	{
		config().blackboard->set<int>("ANIMFLAG", 0);
		config().blackboard->set<float>("HOLDTIME", 0.f);
		config().blackboard->set<std::string>("CURRENTANIM", "Left");
	}

	BT::NodeStatus Hold_AttackAnim::tick()
	{
		// 대기 시간
		auto holdTime = getInput<float>("_holdTime");
		float _value = holdTime.value();

		int flag = config().blackboard->get<int>("ANIMFLAG");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");	// 거쳐온 애니매이션 노드가 무엇인지

		if (flag == 1 && currentAnim.find("Idle") != std::string::npos)
			return BT::NodeStatus::FAILURE;

		if (_value >= 1.0f)
		{
			_value = 0.f;
			setOutput<float>("_holdTime", _value);

			if (flag == 1 && currentAnim.find("Walk") != std::string::npos)
				config().blackboard->set<bool>("ISCHANGE", false);

			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			setOutput<float>("_holdTime", _value);
			return BT::NodeStatus::SUCCESS;
		}
	}
}