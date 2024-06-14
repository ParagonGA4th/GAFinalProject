#include "Anim_gIdle.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_gIdle::tick()
	{
		bool hitInit = config().blackboard->get<bool>("HITINIT");
		bool isAnimEnd = config().blackboard->get<bool>("ISCOOLDOWNANIMEND");

		if (hitInit && !isAnimEnd)
		{
			config().blackboard->set<bool>("ISCHANGE", false);
			config().blackboard->set<std::string>("PREVANIM", "");
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			bool isChange = config().blackboard->get<bool>("ISCHANGE");
			config().blackboard->set<std::string>("CURRENTANIM", "_00001");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			if (isChange || tMeshRenderer->GetAnimation() == animId)
			{
				return BT::NodeStatus::FAILURE;
			}
			else if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, true);
				config().blackboard->set<bool>("ISCHANGE", false);

				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}