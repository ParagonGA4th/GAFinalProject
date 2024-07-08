#include "Anim_Evasion.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Evasion::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossState == Pg::Data::BossState::EVASION) _isParttern = true;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				if (_isParttern)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK;
					_isParttern = false;
				}
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00012.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}