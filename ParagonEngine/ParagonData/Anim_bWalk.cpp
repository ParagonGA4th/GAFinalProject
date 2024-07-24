#include "Anim_bWalk.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_bWalk::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{				
			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00002.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::CHASE;
					tMeshRenderer->SetAnimation(animId, true);
				}
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}