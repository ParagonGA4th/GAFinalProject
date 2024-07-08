#include "Anim_SpinAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_SpinAttack::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (isHolding) return BT::NodeStatus::FAILURE;

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._bossState != Pg::Data::GolemBossState::SKILL_ATTACK)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				return BT::NodeStatus::FAILURE;
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00009.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);

				std::string objName = this->GetGameObject()->GetName();
				objName = objName.substr(0, objName.rfind("_"));
				objName.append("_Crtstal");

				auto tchild = this->GetGameObject()->_transform.GetChild(objName);
				auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

				animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_10009.pganim");
				tcMeshRenderer->SetAnimation(animId, false);
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}