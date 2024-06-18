#include "Anim_RightArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	void Anim_RightArmAttack::InitCustom()
	{
		config().blackboard->set<bool>("ISRANIMEND", false);
	}

	BT::NodeStatus Anim_RightArmAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			bool isAnimEnd = config().blackboard->get<bool>("ISRANIMEND");
			if (isAnimEnd) return BT::NodeStatus::FAILURE;
			if (monHelper->_isAnimationEnd)
			{
				if (!isAnimEnd)
				{
					monHelper->_isAnimationEnd = false;
					config().blackboard->set<bool>("ISRANIMEND", true);
				}
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "_00005");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00005.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
				config().blackboard->set<bool>("ISCHANGE", true);
			}
		}
		
		return BT::NodeStatus::SUCCESS;
	}
}