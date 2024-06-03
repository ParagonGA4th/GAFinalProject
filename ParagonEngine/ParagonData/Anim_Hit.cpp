#include "Anim_Hit.h"
#include "MonsterHelper.h"
#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Data::BTree::Node
{
	void Anim_Hit::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();
	}

	BT::NodeStatus Anim_Hit::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isHit)
			{
				auto holdTime = getInput<float>("_holdTime");

				if (holdTime.value() - _value <= 0.f)
				{
					_value = 0.f;
					monHelper->_isHit = false;
					return BT::NodeStatus::FAILURE;
				}
				else
				{
					auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
					if (tMeshRenderer != nullptr)
					{
						std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
						animId.append("_00003.pganim");

						if (tMeshRenderer->GetAnimation() != animId) tMeshRenderer->SetAnimation(animId, true);
					}

					_value += _deltaTime->GetDeltaTime();
					return BT::NodeStatus::SUCCESS;
				}
			}
		}

		return BT::NodeStatus::FAILURE;
	}
}