#include "Anim_GroundIdle.h"
#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_GroundIdle::tick()
	{
		bool hitInit = config().blackboard->get<bool>("HITINIT");
		bool isChange = config().blackboard->get<bool>("ISCHANGE");
		std::string prevAnim = config().blackboard->get<std::string>("PREVANIM");

		if (hitInit && (prevAnim.empty() || prevAnim.find("_00001") == std::string::npos))
		{
			config().blackboard->set<bool>("ISCHANGE", true);
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
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