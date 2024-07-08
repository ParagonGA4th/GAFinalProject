#include "Anim_gbTwoArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_gbTwoArmAttack::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (isHolding) return BT::NodeStatus::FAILURE;

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._bossState != Pg::Data::GolemBossState::BASIC_ATTACK_3)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimChange = false;
				monHelper->_isAnimationEnd = false;

				monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::IDLE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00008.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
					monHelper->_isAnimChange = true;

					std::string objName = this->GetGameObject()->GetName();
					objName = objName.substr(0, objName.rfind("_"));
					objName.append("_Crtstal");

					auto tchild = this->GetGameObject()->_transform.GetChild(objName);
					auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

					animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
					animId.append("_10008.pganim");
					tcMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}