#include "Anim_Cast.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Cast::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossState != Pg::Data::BossState::CAST)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_bossFlag._bossState = Pg::Data::BossState::BASIC_ATTACK_1;
				return BT::NodeStatus::FAILURE;
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00014.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}