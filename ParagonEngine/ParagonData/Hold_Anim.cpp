#include "Hold_Anim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>

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
		//config().blackboard->set<int>("NUMATT", 3);
		//config().blackboard->set<float>("REPEAT", 2.f);
	}

	BT::NodeStatus Hold_Anim::tick()
	{
		//auto holdTime = getInput<float>("_holdIdleTime");
		////BT_VALIDATE(holdTime);

		if (_value >= 3.f)
		{
			_value = 0.f;
			PG_TRACE("Hold_Anim");
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}