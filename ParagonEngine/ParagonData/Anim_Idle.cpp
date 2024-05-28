#include "Anim_Idle.h"
#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			bool isChange;
			bool isFind = config().blackboard->get<bool>("ISFINDPLAYER");

			if (!isFind)
			{
				config().blackboard->set<std::string>("DECURRENTANIM", "_00001");
				isChange = config().blackboard->get<bool>("ISDECHANGE");
			}
			else
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00001");
				isChange = config().blackboard->get<bool>("ISCHANGE");
			}

			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			if (isChange || tMeshRenderer->GetAnimation() == animId)
			{
				return BT::NodeStatus::FAILURE;
			}
			else if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, true);

				if (!isFind) config().blackboard->set<bool>("ISDECHANGE", false);
				else config().blackboard->set<bool>("ISCHANGE", false);
				
				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}