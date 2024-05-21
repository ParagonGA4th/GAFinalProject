#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<float>("HOLDTIME", 0.f);
	}

	BT::NodeStatus Anim_Delay::tick()
	{
		auto holdTime = getInput<float>("_holdTime");
		float _value = holdTime.value();

		if (_value >= 1.0f)
		{
			_value = 0.f;
			setOutput<float>("_holdTime", _value);
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