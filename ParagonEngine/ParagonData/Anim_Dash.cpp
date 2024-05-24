#include "Anim_Dash.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Dash::tick()
	{
		auto children = this->GetGameObject()->_transform.GetChildren();
		for (auto& child : children)
		{
			if (child->_object->GetComponent<Pg::Data::MonsterHelper>() != nullptr)
			{
				auto goTo = child->_object->GetComponent<Pg::Data::MonsterHelper>();
				if (goTo->_isAnimationEnd)
				{
					config().blackboard->set<bool>("ANIMEND", true);
					goTo->_isAnimationEnd = false;
				}
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00003.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
				return BT::NodeStatus::SUCCESS;
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}