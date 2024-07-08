#include "Anim_RightArmAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_RightArmAttack::tick()
	{
		bool isHolding = config().blackboard->get<bool>("ISHOLDING");
		if (isHolding) return BT::NodeStatus::FAILURE;

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossState != Pg::Data::BossState::BASIC_ATTACK_1)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimChange = false;
				monHelper->_isAnimationEnd = false;

				if (_isCast)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::BASIC_ATTACK_2;
					_isCast = false;
				}
				else _isCast = true;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				if (_isCast) animId.append("_00005.pganim");
				else animId.append("_00014.pganim"); // 캐스팅 애니매이션

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);
					if(_isCast) monHelper->_isAnimChange = true;
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}