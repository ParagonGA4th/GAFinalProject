#include "Anim_FlyAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_FlyAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossStateListByEnum[monHelper->_bossState].find("SFlyAttack") == std::string::npos)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;

				switch (monHelper->_bossState)
				{
				case Pg::Data::BossState::SKILL_FLY_ATTACK_1:
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1:
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_2;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_2:
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2:
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_3;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_3:
					monHelper->_bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3:
					monHelper->_bossState = Pg::Data::BossState::IDLE;
					break;
				}

				config().blackboard->set<std::string>
					("NEXTANIM", monHelper->_bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_1]);
			}


			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00009");
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));

				if (monHelper->_bossStateListByEnum[monHelper->_bossState].find("Pre") == std::string::npos)
					animId.append("_00009.pganim");
				else
					animId.append("_00009.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
				}
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}