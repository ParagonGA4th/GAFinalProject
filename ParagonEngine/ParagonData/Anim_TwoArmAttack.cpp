#include "Anim_TwoArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_TwoArmAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossState != Pg::Data::BossState::BASIC_ATTACK_3)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimChange = false;
				monHelper->_isAnimationEnd = false;

				monHelper->_bossFlag._bossState = Pg::Data::BossState::CAST;

				if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1 && !_isNextPattern) _isNextPattern = true;
				else if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1 && _isNextPattern)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::DASH;
					_isNextPattern = false;
				}
				//else if(monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3) monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00007.pganim");

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