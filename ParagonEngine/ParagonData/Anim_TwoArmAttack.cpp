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
			if (monHelper->_bossState != Pg::Data::BossState::BASIC_ATTACK_3) return BT::NodeStatus::FAILURE;
			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_isAnimChange = false;
				monHelper->_bossState = Pg::Data::BossState::IDLE;
				config().blackboard->set<std::string>
					("NEXTANIM", monHelper->_bossStateListByEnum[Pg::Data::BossState::BASIC_ATTACK_1]);
				if (monHelper->_bossPase == Pg::Data::BossPase::PASE_2)
				{
					if (_isSkillTime)
					{
						config().blackboard->set<std::string>
							("NEXTANIM", monHelper->_bossStateListByEnum[Pg::Data::BossState::SKILL_FEATHER_ATTACK]);
					}

					_isSkillTime = !_isSkillTime;
				}
			}


			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00007");
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