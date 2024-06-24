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
				_isAnimEnd = true;
				monHelper->_isAnimationEnd = false;
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			_animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));

			if (downInit && (_isInit || !_isInit))
			{
				_isInit = true;
				_animId.append("_00011.pganim");
			}
			else if ((_isInit && _isAnimEnd) || _isLoading)
			{
				_isInit = false;
				_isLoading = true;
				_animId.append("_00015.pganim");
			}
			else if ((_isLoading && _isAnimEnd) || _isEnd)
			{
				_isLoading = false;
				_isEnd = true;
				config().blackboard->set<bool>("DOWNINIT", false);
				_animId.append("_00016.pganim");
			}

			if (tMeshRenderer->GetAnimation() != _animId)
			{
				tMeshRenderer->SetAnimation(_animId, _isLoading);
				_isAnimEnd = false;
			}
		}

		if (_isEnd && _isAnimEnd) return BT::NodeStatus::FAILURE;
		else return BT::NodeStatus::SUCCESS;
	}
}