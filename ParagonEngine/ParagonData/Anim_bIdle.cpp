#include "Anim_bIdle.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>


namespace Pg::Data::BTree::Node
{
	void Anim_bIdle::InitCustom()
	{
		_deltaTime = &singleton<Pg::Util::Time::TimeSystem>();
		config().blackboard->set<std::string>("NEXTANIM", "");
	}

	BT::NodeStatus Anim_bIdle::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossState != Pg::Data::BossState::IDLE) return BT::NodeStatus::FAILURE;
			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				config().blackboard->set<std::string>("CURRENTANIM", "_00001");
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00001.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, true);
				}
			}

			auto holdTime = getInput<float>("_holdTime");

			if (holdTime.value() == -1.f) return BT::NodeStatus::SUCCESS;
			if (holdTime.value() - _value <= 0.f)
			{
				_value = 0.f;

				std::string state = config().blackboard->get<std::string>("NEXTANIM");
				monHelper->_bossState = monHelper->_bossStateListByString[state];
				config().blackboard->set<std::string>("NEXTANIM", "");
				return BT::NodeStatus::FAILURE;
			}
			else
			{
				_value += _deltaTime->GetDeltaTime();
				return BT::NodeStatus::SUCCESS;
			}

		}

		return BT::NodeStatus::FAILURE;
	}
}