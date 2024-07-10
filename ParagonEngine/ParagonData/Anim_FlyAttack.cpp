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
			if (monHelper->_bossFlag._bossStateListByEnum[monHelper->_bossFlag._bossState].find("SFlyAttack") == std::string::npos)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;

				switch (monHelper->_bossFlag._bossState)
				{
				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_1:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_2:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_2;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_2:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_3:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_3;
					break;

				case Pg::Data::BossState::SKILL_FLY_ATTACK_3:
					monHelper->_bossFlag._bossState = Pg::Data::BossState::IDLE;
					break;
				}
			}


			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));

				if (monHelper->_bossFlag._bossStateListByEnum[monHelper->_bossFlag._bossState].find("Pre") == std::string::npos)
					animId.append("_00013.pganim");
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