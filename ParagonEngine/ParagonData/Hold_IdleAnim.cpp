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

	BT::NodeStatus Hold_IdleAnim::tick()
	{
		//if (_isReturn)
		//{
		//	if (_value <= 0.f)
		//	{
		//		_isReturn = false;
		//		PG_TRACE("Hold_IdleAnimR");
		//		return BT::NodeStatus::FAILURE;
		//	}
		//	else
		//	{
		//		_value -= _deltaTime->GetDeltaTime();
		//		return BT::NodeStatus::SUCCESS;
		//	}

		//}
		//else
		//{
		PG_TRACE("Hold_IdleAnim");

		if (_value >= 3.f)
		{
			_isReturn = true;
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
		//}
	}
}