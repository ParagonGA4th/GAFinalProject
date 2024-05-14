#include "Hold_IdleAnim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	Hold_IdleAnim::Hold_IdleAnim(const std::string& name, const BT::NodeConfiguration& config)
		: BT::SyncActionNode(name, config)
	{
		auto& tdelta = singleton<Pg::Util::Time::TimeSystem>();
		_deltaTime = &tdelta;
	}

	//void Hold_IdleAnim::InitCustom()
	//{
		//config().blackboard->set<float>("HOLDIDLE", 0.f);
		//config().blackboard->set<float>("REPEAT", 2.f);
	//}

	BT::NodeStatus Hold_IdleAnim::tick()
	{
		//auto holdTime = getInput<float>("_holdIdleTime");
		////BT_VALIDATE(holdTime);

		//if (!_isInit)
		//{
		//	_value = holdTime.value();
		//	_isInit = true;
		//}

		//if (_value >= 2.f)
		//{
		//	_isInit = false;
		//	return BT::NodeStatus::FAILURE;
		//}
		//else
		//{
		//	_value += _deltaTime->GetDeltaTime();
		//	return BT::NodeStatus::SUCCESS;
		//}

		return BT::NodeStatus::RUNNING;
	}
}