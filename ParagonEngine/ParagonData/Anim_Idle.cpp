#include "Anim_Idle.h"

#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			if (tMeshRenderer->GetAnimation() != "PpakMonster_Idle.pganim")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Idle.pganim", true);
				PG_TRACE("Anim_Idle_SUCCESS");
				//return BT::NodeStatus::SUCCESS;
			}
			//else
			//{
			//	PG_TRACE("Anim_Idle_FAILURE");
			//	return BT::NodeStatus::FAILURE;
			//}
		}
		return BT::NodeStatus::SUCCESS;
	}
}