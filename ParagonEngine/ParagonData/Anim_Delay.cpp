#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<bool>("ISCHANGE", false);
		config().blackboard->set<std::string>("CURRENTANIM", "_00001");

	}

	BT::NodeStatus Anim_Delay::tick()
	{
		auto holdTime = getInput<float>("_holdTime");

		bool isChange = config().blackboard->get<bool>("ISCHANGE");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");	// 거쳐온 애니매이션 노드가 무엇인지

		// Idle이 아닌 다른 애니매이션 이름
		if (_otherAnim.empty() && currentAnim.find("_00001") == std::string::npos)
		{
			_otherAnim = currentAnim;
		}

		if (isChange && currentAnim.find("_00001") != std::string::npos)
		{
			return BT::NodeStatus::FAILURE;
		}

		if (holdTime.value() - _value <= 0.f)
		{
			_value = 0.f;
			if (isChange && currentAnim.find(_otherAnim) != std::string::npos)
				config().blackboard->set<bool>("ISCHANGE", false);

			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}