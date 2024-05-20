#include "Anim_Idle.h"
#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"
#include <string>

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "Idle");
			bool isChange = config().blackboard->get<bool>("ISCHANGE");

			if (isChange || tMeshRenderer->GetAnimation() == "PpakMonster_Idle.pganim")
			{
				PG_TRACE("Idle_Load");
				return BT::NodeStatus::FAILURE;
			}
			else if(tMeshRenderer->GetAnimation() != "PpakMonster_Idle.pganim")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Idle.pganim", true);
				config().blackboard->set<bool>("ISCHANGE", false);

				PG_TRACE("Idle_Set");

				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}