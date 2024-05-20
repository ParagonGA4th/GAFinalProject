#include "Hold_Anim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>

#include <string>

namespace Pg::Data::BTree::Node
{
	Hold_Anim::Hold_Anim(const std::string& name, const BT::NodeConfiguration& config)
		: BT::SyncActionNode(name, config)
	{
		auto& tdelta = singleton<Pg::Util::Time::TimeSystem>();
		_deltaTime = &tdelta;
	}

	void Hold_Anim::InitCustom()
	{
		config().blackboard->set<bool>("ISCHANGE", false);
		config().blackboard->set<float>("HOLDTIME", 0.f);
		config().blackboard->set<std::string>("CURRENTANIM", "Idle");
	}

	BT::NodeStatus Hold_Anim::tick()
	{
		auto holdTime = getInput<float>("_holdTime");
		_value = holdTime.value();

		PG_TRACE(_value);

		bool isChange = config().blackboard->get<bool>("ISCHANGE");
		std::string currentAnim = config().blackboard->get<std::string>("CURRENTANIM");

		if (isChange && currentAnim.find("Idle") != std::string::npos)
		{
			PG_TRACE("Now Anim Is Walk");
			return BT::NodeStatus::FAILURE;
		}

		if (_value >= 1.0f)
		{
			_value = 0.f;
			setOutput<float>("_holdTime", _value);
			
			if (isChange && currentAnim.find("Walk") != std::string::npos)
			{
				PG_TRACE("Now Anim Is Idle");
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