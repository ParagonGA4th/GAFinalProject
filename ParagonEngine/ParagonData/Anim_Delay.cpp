#include "Anim_Delay.h"
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>
#include <limits.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Delay::InitCustom()
	{
		config().blackboard->set<bool>("ISHOLDING", false);
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();
	}

	BT::NodeStatus Anim_Delay::tick()
	{

		auto holdTime = getInput<float>("_holdTime");				  // ����Ϸ��� �ð�
		bool isHolding = config().blackboard->get<bool>("ISHOLDING"); // ������ΰ�?

		if (!isHolding) config().blackboard->set<bool>("ISHOLDING", true); // ȣ��Ǿ��� ������ ��� ��

		if (holdTime.value() - _value <= 0.f)
		{
			_value = 0.0f; // ��� ����
			config().blackboard->set<bool>("ISHOLDING", false);
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			_value += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}

	}
}