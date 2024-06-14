#include "Anim_PoisonGas.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_PoisonGas::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				config().blackboard->set<bool>("ISSKILLANIMEND", true);
				config().blackboard->set<bool>("ISCHANGE", false);
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