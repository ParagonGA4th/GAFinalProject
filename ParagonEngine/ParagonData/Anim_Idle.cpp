#include "Anim_Idle.h"

#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			if (tMeshRenderer->GetAnimation() != "PpakMonster_Idle")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Idle", true);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}