#include "Anim_bDead.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_bDead::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isDeadDelay = true;
				monHelper->_isAnimationEnd = false;
				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00004.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}