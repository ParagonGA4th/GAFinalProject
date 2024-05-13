#include "Anim_Walk.h"

#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Walk::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			if (tMeshRenderer->GetAnimation() != "PpakMonster_Punch")
			{
				tMeshRenderer->SetAnimation("PpakMonster_Punch", true);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}