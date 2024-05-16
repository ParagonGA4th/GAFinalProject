#include "Anim_Walk.h"
#include "SkinnedMeshRenderer.h"

//#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/Log.h"
//#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	//auto& tdelta = singleton<Pg::Util::Time::TimeSystem>();
	//_deltaTime = &tdelta;

	BT::NodeStatus Anim_Walk::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			if (tMeshRenderer->GetAnimation() != "PpakMonster_Punch.pganim")
			{
				PG_TRACE("Anim_Walk");
				tMeshRenderer->SetAnimation("PpakMonster_Punch.pganim", true);
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}