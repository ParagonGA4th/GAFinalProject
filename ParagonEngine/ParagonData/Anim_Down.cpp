#include "Anim_Down.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Down::tick()
	{
		bool downInit = config().blackboard->get<bool>("DOWNINIT");
		if (!downInit) return BT::NodeStatus::FAILURE;

		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				if (monHelper->_bossFlag._bossState != Pg::Data::BossState::DOWNENDED)
					monHelper->_isAnimationEnd = false;

				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string _animId;
				bool _loop = false;
				_animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));

				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::DOWN)
				{
					if (!_isInit) _animId.append("_00011.pganim");
					else _animId.append("_00015.pganim"); _loop = true;
				}

				if (monHelper->_bossFlag._bossState == Pg::Data::BossState::DOWNENDED)
				{
					_animId.append("_00016.pganim");
				}

				if (tMeshRenderer->GetAnimation() != _animId)
				{
					tMeshRenderer->SetAnimation(_animId, _loop);
				}
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}