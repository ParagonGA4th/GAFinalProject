#include "Anim_Walk.h"

#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Walk::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			PG_TRACE("Anim_Walk");

			if (tMeshRenderer->GetAnimation() != "PpakMonster_Punch.pganim")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Punch.pganim", true);
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