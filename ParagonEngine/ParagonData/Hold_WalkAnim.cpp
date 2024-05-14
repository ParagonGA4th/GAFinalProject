#include "Hold_WalkAnim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	Hold_WalkAnim::Hold_WalkAnim(const std::string& name, const BT::NodeConfiguration& config)
		: BT::SyncActionNode(name, config)
	{
		auto& tdelta = singleton<Pg::Util::Time::TimeSystem>();
		_deltaTime = &tdelta;
	}

	//void Hold_WalkAnim::InitCustom()
	//{
	//	config().blackboard->set<float>("HOLDWALK", 0.f);
	//	//config().blackboard->set<float>("REPEAT", 2.f);
	//}

	BT::NodeStatus Hold_WalkAnim::tick()
	{
		//auto holdTime = getInput<float>("_holdWalkTime");
		//BT_VALIDATE(holdTime);

		if (!_isInit)
		{
			_value = 0.f;
			_isInit = true;
		}

		if (_value >= 2.f)
		{
			_isInit = false;
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}