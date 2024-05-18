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
		config().blackboard->set<float>("HOLDTIME", 0.f);
		//config().blackboard->set<float>("REPEAT", 2.f);
	}

	BT::NodeStatus Hold_Anim::tick()
	{
		auto holdTime = getInput<float>("_holdTime");
		_value = holdTime.value();

		//if (_isReturn)
		//{
		//	if (_value <= 0.f)
		//	{
		//		_isReturn = false;
		//		setOutput<float>("_holdTime", _value);
		//		PG_TRACE("Hold_Reverse");
		//		return BT::NodeStatus::FAILURE;
		//	}
		//	else
		//	{
		//		_value -= _deltaTime->GetDeltaTime();
		//		setOutput<float>("_holdTime", _value);
		//		return BT::NodeStatus::SUCCESS;
		//	}

		//}

		PG_TRACE(_value);
			
		if (_value >= 1.5f)
		{
			_value = 0.f;
			//PG_TRACE("END_Hold");
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