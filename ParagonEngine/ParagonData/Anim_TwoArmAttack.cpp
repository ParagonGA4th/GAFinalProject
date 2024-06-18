#include "Anim_TwoArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void Anim_TwoArmAttack::InitCustom()
	{
		config().blackboard->set<bool>("ISTANIMEND", false);
	}

	BT::NodeStatus Anim_TwoArmAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			bool isAnimEnd = config().blackboard->get<bool>("ISTANIMEND");
			if (isAnimEnd) return BT::NodeStatus::FAILURE;
			if (monHelper->_isAnimationEnd)
			{
				if (!isAnimEnd)
				{
					monHelper->_isAnimationEnd = false;
					config().blackboard->set<bool>("ISTANIMEND", true);
					config().blackboard->set<std::string>("PREVANIM", "_00007");
				}
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "_00007");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00007.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
				config().blackboard->set<bool>("ISCHANGE", true);
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}