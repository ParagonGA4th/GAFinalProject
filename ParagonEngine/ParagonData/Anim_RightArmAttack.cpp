#include "Anim_RightArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_RightArmAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossState != Pg::Data::BossState::BASIC_ATTACK_1) return BT::NodeStatus::FAILURE;
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimChange = false;
				monHelper->_isAnimationEnd = false;

				//if (_isCast)
				monHelper->_bossState = Pg::Data::BossState::BASIC_ATTACK_2;
			}


			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00005");
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				if (_isCast) animId.append("_00005.pganim");
				else animId.append("_00005.pganim"); // 캐스팅 애니매이션

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
					monHelper->_isAnimChange = true;
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}