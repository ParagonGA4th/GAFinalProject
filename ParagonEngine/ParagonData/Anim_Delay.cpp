#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();

		config().blackboard->set<bool>("ISCHANGE", false);
		config().blackboard->set<std::string>("CURRENTANIM", "");
		config().blackboard->set<std::string>("PREVANIM", "");
	}

	BT::NodeStatus Anim_Delay::tick()
	{
		auto holdTime = getInput<float>("_holdTime");

		bool isChange = config().blackboard->get<bool>("ISCHANGE");	// 애니매이션이 바뀌었는지
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");	// 현재 애니매이션 노드가 무엇인지
		std::string prevAnim = config().blackboard->get<std::string>("PREVANIM");		// 이전 애니매이션 노드가 무엇인지

		if (prevAnim.empty())
		{
			config().blackboard->set<std::string>("PREVANIM", currentAnim);
		}

		if (isChange && prevAnim == currentAnim)
		{
			return BT::NodeStatus::FAILURE;
		}

		if (holdTime.value() - _value <= 0.f)
		{
			_value = 0.f;
			if (isChange && prevAnim != currentAnim)
			{
				config().blackboard->set<bool>("ISCHANGE", false);
				config().blackboard->set<std::string>("PREVANIM", "");
			}

			if (currentAnim.find("_00004") != std::string::npos
				|| currentAnim.find("_00003") != std::string::npos)
			{
				config().blackboard->set<bool>("ISCHANGE", true);
				config().blackboard->set<std::string>("PREVANIM", "");
			}

			if (GetGameObject()->GetName().find("Stub") != std::string::npos)
			{
				config().blackboard->set<bool>("COOLINIT", false);
			}

			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}