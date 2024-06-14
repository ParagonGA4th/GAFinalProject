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
				_isChangeAnim = false;
				monHelper->_isAnimationEnd = false;

				config().blackboard->set<bool>("ISCHANGE", false);
			}
			else
			{
				if (monHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN)
				{
					if (_isChangeAnim) _isChangeAnim = false;
				}
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00004");
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00004.pganim");

				if (!_isChangeAnim &&
					(monHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN ||
					 monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_1 ||
					 monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_2 ||
					 monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_3 ))
				{
					if (monHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN) monHelper->_trentState = Pg::Data::TrentState::BASIC_ATTACK_1;
					else if (monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_1) monHelper->_trentState = Pg::Data::TrentState::BASIC_ATTACK_2;
					else if (monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_2) monHelper->_trentState = Pg::Data::TrentState::BASIC_ATTACK_3;
					else if (monHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_3) monHelper->_trentState = Pg::Data::TrentState::BASICATTACK_COOLDOWN;

					_isChangeAnim = true;
					tMeshRenderer->SetAnimation(animId, false);
					config().blackboard->set<bool>("ISCHANGE", true);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}