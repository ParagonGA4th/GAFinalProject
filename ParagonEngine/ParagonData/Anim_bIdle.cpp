#include "Anim_bIdle.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"


namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_bIdle::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
		 	if (isHolding)
			{
				monHelper->_bossFlag._bossState = Pg::Data::BossState::CASTING;
				if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_2)
				{
					if (_isSPSkillTime)
						monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK;

					_isSPSkillTime = !_isSPSkillTime;
				}

				if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3)
				{
					if (_isTPSkillTime >= 3)
					{
						monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK;
						_isTPSkillTime = 0; // Reset
					}

					_isTPSkillTime++;
				}
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, true);
			}
		}
		return BT::NodeStatus::FAILURE;
	}
}