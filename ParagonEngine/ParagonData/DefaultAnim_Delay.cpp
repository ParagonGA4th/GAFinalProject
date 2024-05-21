#include "DefaultAnim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void DefaultAnim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<bool>("ISDECHANGE", false);
		config().blackboard->set<std::string>("DECURRENTANIM", "_00001");
	}

	BT::NodeStatus DefaultAnim_Delay::tick()
	{
		// 대기 시간
		auto holdTime = getInput<float>("_deHoldTime");

		bool isChange = config().blackboard->get<bool>("ISDECHANGE");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("DECURRENTANIM");	// 거쳐온 애니매이션 노드가 무엇인지

		if (isChange && currentAnim.find("_00001") != std::string::npos)
		{
			return BT::NodeStatus::FAILURE;
		}

		if (holdTime.value() - _value <= 0.f)
		{
			_value = 0.f;
			if (isChange && currentAnim.find("_00002") != std::string::npos)
				config().blackboard->set<bool>("ISDECHANGE", false);

			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}
