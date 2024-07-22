#include "Anim_ThrowCoin.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_ThrowCoin::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (isHolding) return BT::NodeStatus::SUCCESS;

		auto monHelper = GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_state = Pg::Data::MonsterState::ATTACK;
				monHelper->_isAnimationEnd = false;
			}
		}

		auto tMeshRenderer = GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00005.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}