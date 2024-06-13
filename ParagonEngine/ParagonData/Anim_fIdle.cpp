#include "Anim_fIdle.h"
#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_fIdle::tick()
	{
		bool hitInit = config().blackboard->get<bool>("HITINIT");
		std::string prevAnim = config().blackboard->get<std::string>("PREVANIM");

		if (hitInit && prevAnim.empty()) 
		{
			config().blackboard->set<bool>("ISCHANGE", true);
			config().blackboard->set<std::string>("PREVANIM", "_00001");
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			bool isChange = config().blackboard->get<bool>("ISCHANGE");
			config().blackboard->set<std::string>("CURRENTANIM", "_00001");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			if (isChange || tMeshRenderer->GetAnimation() == animId)
			{
				return BT::NodeStatus::FAILURE;
			}
			else if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, true);
				config().blackboard->set<bool>("ISCHANGE", false);

				std::string objName = this->GetGameObject()->GetName();
				objName = objName.substr(0, objName.rfind("_"));
				if (objName.find("Golem") != std::string::npos) objName.append("_Crtstal");
				else objName.append("_Wing");

				auto tchild = this->GetGameObject()->_transform.GetChild(objName);
				auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

				animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_10001.pganim");
				tcMeshRenderer->SetAnimation(animId, false);

				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}