#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<bool>("ISCHANGE", false);
		config().blackboard->set<float>("HOLDTIME", 0.f);
		config().blackboard->set<std::string>("CURRENTANIM", "_00001");
	}

	BT::NodeStatus Anim_Delay::tick()
	{
		// 대기 시간
		auto holdTime = getInput<float>("_holdTime");
		float _value = holdTime.value();

		bool isChange = config().blackboard->get<bool>("ISCHANGE");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");	// 거쳐온 애니매이션 노드가 무엇인지

		if (isChange && currentAnim.find("Idle") != std::string::npos)
		{
			return BT::NodeStatus::FAILURE;
		}

		if (_value >= 1.0f)
		{
			_value = 0.f;
			setOutput<float>("_holdTime", _value);
			
			if (isChange && currentAnim.find("Walk") != std::string::npos)
			{
				config().blackboard->set<bool>("ISCHANGE", false);
			}

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
