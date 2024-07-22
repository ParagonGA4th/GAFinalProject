#include "Anim_DownEnd.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_DownEnd::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1)
				{
					monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_2;
				}
				else if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_2)
				{
					monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3;
				}
				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00016.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
				}
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}