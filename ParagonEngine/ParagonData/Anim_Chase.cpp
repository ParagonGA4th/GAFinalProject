#include "Anim_Chase.h"
#include "SkinnedMeshRenderer.h"

#include "../ParagonUtil/Log.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Chase::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "Walk");
			bool isChange = config().blackboard->get<bool>("ISCHANGE");

			if (!isChange && tMeshRenderer->GetAnimation() != "PpakMonster_Punch.pganim")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Punch.pganim", true);
				config().blackboard->set<bool>("ISCHANGE", true);
				return BT::NodeStatus::SUCCESS;
			}
			else if (isChange && tMeshRenderer->GetAnimation() == "PpakMonster_Punch.pganim")
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}