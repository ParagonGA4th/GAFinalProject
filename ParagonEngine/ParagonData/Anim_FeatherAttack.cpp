#include "Anim_FeatherAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_FeatherAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossStateListByEnum[monHelper->_bossFlag._bossState].find("SFeatherAttack") == std::string::npos)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FEATHER_ATTACK_PREPARE)
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK;

				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FEATHER_ATTACK)
				{
					_isInit = false;
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK_END;
				}

				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FEATHER_ATTACK_END)
					monHelper->_bossFlag._bossState = Pg::Data::BossState::IDLE;	

				monHelper->_isAnimationEnd = false;

				return BT::NodeStatus::FAILURE;
			}


			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				bool _loop = false;
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				if (!_isInit) animId.append("_00008.pganim");
				else animId.append("_00017.pganim"); _loop = true;

				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::SKILL_FEATHER_ATTACK_END)
					animId.append("_00018.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, _loop);
				}
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}