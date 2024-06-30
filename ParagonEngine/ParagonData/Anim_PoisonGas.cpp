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
			if (monHelper->_stubFlag._stubState != Pg::Data::StubState::IDLE &&
				monHelper->_stubFlag._stubState != Pg::Data::StubState::BASICATTACK_COOLDOWN &&
				monHelper->_stubFlag._stubState != Pg::Data::StubState::SKILL_ATTACK)
			{
				return BT::NodeStatus::FAILURE;
			}

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_stubFlag._stubState = Pg::Data::StubState::SKILL_COOLDOWN;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00005.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					monHelper->_stubFlag._stubState = Pg::Data::StubState::SKILL_ATTACK;
					tMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}