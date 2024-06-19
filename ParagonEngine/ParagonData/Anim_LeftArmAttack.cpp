#include "Anim_LeftArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_LeftArmAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossState != Pg::Data::BossState::BASIC_ATTACK_2) 
				return BT::NodeStatus::FAILURE;
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_bossState = Pg::Data::BossState::BASIC_ATTACK_3;
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "_00006");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00006.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
				config().blackboard->set<bool>("ISCHANGE", true);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}