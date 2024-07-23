#include "Anim_tAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_tAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_isAnimChange = false;

				if (monHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_3)
				{
					monHelper->_stubFlag._stubState = Pg::Data::StubState::BASICATTACK_COOLDOWN;
				}
				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00004.pganim");

				if (!monHelper->_isAnimChange &&
					(monHelper->_stubFlag._stubState == Pg::Data::StubState::SKILL_COOLDOWN ||
						monHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_1 ||
						monHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_2))
				{
					if (monHelper->_stubFlag._stubState == Pg::Data::StubState::SKILL_COOLDOWN) monHelper->_stubFlag._stubState = Pg::Data::StubState::BASIC_ATTACK_1;
					else if (monHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_1) monHelper->_stubFlag._stubState = Pg::Data::StubState::BASIC_ATTACK_2;
					else if (monHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_2) monHelper->_stubFlag._stubState = Pg::Data::StubState::BASIC_ATTACK_3;

					monHelper->_isAnimChange = true;
					tMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}