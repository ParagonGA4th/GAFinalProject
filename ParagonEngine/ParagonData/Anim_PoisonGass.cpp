#include "Anim_PoisonGass.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_PoisonGass::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				_isAnimEnd = true;
				monHelper->_isAnimationEnd = false;
			}
			else
			{
				_isAnimEnd = false;
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

		if (_isAnimEnd) config().blackboard->set<bool>("ISCHANGE", false);

		return BT::NodeStatus::SUCCESS;
	}
}