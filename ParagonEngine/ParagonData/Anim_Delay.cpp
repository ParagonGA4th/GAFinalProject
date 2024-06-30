#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>
#include <limits.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		config().blackboard->set<bool>("ISHOLDING", false);
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();
	}

	BT::NodeStatus Anim_Delay::tick()
	{
		auto holdTime = getInput<float>("_holdTime");				  // 대기하려는 시간
		bool isHolding = config().blackboard->get<bool>("ISHOLDING"); // 대기중인가?

		if (!isHolding) config().blackboard->set<bool>("ISHOLDING", true); // 호출되었기 때문에 대기 중

		if (holdTime.value() - value <= 0.f)
		{
			value = 0.0f; // 대기 리셋
			config().blackboard->set<bool>("ISHOLDING", false);
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}