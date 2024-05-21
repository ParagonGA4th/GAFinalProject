#include "DefaultAnim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void DefaultAnim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<bool>("ISCHANGE", false);
		config().blackboard->set<float>("DEHOLDTIME", 0.f);
		config().blackboard->set<std::string>("CURRENTANIM", "_00001");
	}

	BT::NodeStatus DefaultAnim_Delay::tick()
	{
		// 대기 시간
		auto holdTime = getInput<float>("_deHoldTime");
		float _value = holdTime.value();

		bool isChange = config().blackboard->get<bool>("ISCHANGE");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");	// 거쳐온 애니매이션 노드가 무엇인지

		if (isChange && currentAnim.find("_00001") != std::string::npos)
		{
			return BT::NodeStatus::FAILURE;
		}

		if (_value >= 1.0f)
		{
			_value = 0.f;
			setOutput<float>("_deHoldTime", _value);
			
			if (isChange && currentAnim.find("_00002") != std::string::npos)
			{
				config().blackboard->set<bool>("ISCHANGE", false);
			}

			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			setOutput<float>("_deHoldTime", _value);
			return BT::NodeStatus::SUCCESS;
		}
	}
}
