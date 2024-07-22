#include "Anim_Shoot.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Shoot::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (isHolding) return BT::NodeStatus::FAILURE;

		auto monHelper = GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_state = Pg::Data::MonsterState::ATTACK;
				monHelper->_waspFlag._attackCount++;
				if (monHelper->_waspFlag._attackCount > 2) monHelper->_waspFlag._attackCount = 0;
				monHelper->_isAnimationEnd = false;
				return BT::NodeStatus::FAILURE;
			}
		}

		auto tMeshRenderer = GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00005.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);

				std::string objName = GetGameObject()->GetName();
				objName = objName.substr(0, objName.rfind("_"));
				objName.append("_Wing");

				auto tchild = GetGameObject()->_transform.GetChild(objName);
				auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

				animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_10005.pganim");
				tcMeshRenderer->SetAnimation(animId, false);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}