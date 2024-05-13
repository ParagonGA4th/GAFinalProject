#include "Hold_Anim.h"

#include "../ParagonData/BtNodes/BTHelper.h"

//#include "../ParagonAPI/PgTime.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	Hold_Anim::Hold_Anim(const std::string& name, const BT::NodeConfiguration& config)
		: BT::SyncActionNode(name, config)
	{
		//auto& tdelta = singleton<Pg::API::Time::PgTime>();
		//_deltaTime = &tdelta;

		_time = 0.f;
	}

	BT::NodeStatus Hold_Anim::tick()
	{
		auto hitSpace = getInput<bool>("_isHitSpace");
		BT_VALIDATE(hitSpace);

		if (_time >= 0.3f)
		{
			_time = 0.f;
			return BT::NodeStatus::FAILURE;
		}
		else
		{
			//_time += _deltaTime->GetDeltaTime();
			return BT::NodeStatus::SUCCESS;
		}
	}
}

