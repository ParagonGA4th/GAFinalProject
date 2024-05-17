#include "Anim_Idle.h"

#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		PG_TRACE("Anim_Idle");
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			if (tMeshRenderer->GetAnimation() != "PpakMonster_Idle.pganim")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Idle.pganim", true);
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}